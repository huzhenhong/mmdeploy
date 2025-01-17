// Copyright (c) OpenMMLab. All rights reserved.
#include "gather.h"

#include "../ncnn_ops_definer.h"
#include "assert.h"

namespace mmdeploy
{
    using namespace ncnn;
    DEFINE_LAYER_CREATOR(Gather)
    DEFINE_NCNN_OPS(Gather, Gather)

    Gather::Gather()
    {
        one_blob_only   = false;
        support_inplace = false;
    }

    int Gather::load_param(const ParamDict& pd)
    {
        axis = pd.get(0, 0);

        return 0;
    }

    // Gather only support 1-dim of indices, because the data and indices all has
    // implicit batch in ncnn, this will lead to wrong shape to match onnx result.
    // When indices dim equals to 1, after eliminating implicit batch, the indices
    // dim still be 1. So there is only 1 implicit batch in data, this will make
    // the shape match onnx result.
    int Gather::forward(const std::vector<Mat>& bottom_blobs,
                        std::vector<Mat>&       top_blobs,
                        const Option&           opt) const
    {
        const Mat& bottom_blob   = bottom_blobs[0];
        const Mat& indices       = bottom_blobs[1];
        int        dims          = bottom_blob.dims;
        int        indices_dims  = indices.dims;
        size_t     elemsize      = bottom_blob.elemsize;
        int        positive_axis = axis < 0 ? dims + axis : axis;
        Mat&       top_blob      = top_blobs[0];
        assert(indices.dims == 1);
        const float* indices_ptr = indices;

        if (dims == 1 && indices_dims == 1)  // positive_axis == 0
        {
            int w = indices.w;
            top_blob.create(w, elemsize, opt.blob_allocator);
            if (top_blob.empty())
            {
                return -100;
            }
            const float* ptr    = bottom_blob;
            float*       outptr = top_blob;
            for (int i = 0; i < w; i++)
            {
                float indice = indices_ptr[i];
                outptr[i]    = ptr[(int)(indice + 0.5)];
            }

            return 0;
        }

        if (dims == 2 && positive_axis == 0 && indices_dims == 1)
        {
            int w = bottom_blob.w;
            int h = bottom_blob.h;
            top_blob.create(w, indices.w, elemsize, opt.blob_allocator);
            // w -> w
            // h -> indices.w
            // h * w -> indices.w * w
            if (top_blob.empty())
            {
                return -100;
            }
            const float* ptr    = bottom_blob;
            float*       outptr = top_blob;
            for (int i = 0; i < indices.w; i++)
            {
                const int selected = (int)(indices_ptr[i] + 0.5);
                memcpy(top_blob.row(i), bottom_blob.row(selected), w * elemsize);
            }

            return 0;
        }

        if (dims == 2 && positive_axis == 1 && indices_dims == 1)
        {
            int w = bottom_blob.w;
            int h = bottom_blob.h;
            top_blob.create(indices.w, h, elemsize, opt.blob_allocator);
            // w -> h
            // h -> indices.w
            // h * w -> indices.w * h
            if (top_blob.empty())
            {
                return -100;
            }
            const float* ptr    = bottom_blob;
            float*       outptr = top_blob;
            for (int j = 0; j < h; j++)
            {
                for (int i = 0; i < indices.w; i++)
                {
                    int selected              = (int)(indices_ptr[i] + 0.5);
                    outptr[j * indices.w + i] = ptr[j * w + selected];
                }
            }
            return 0;
        }

        if (dims == 3 && positive_axis == 0 && indices_dims == 1)
        {
            int w        = bottom_blob.w;
            int h        = bottom_blob.h;
            int channels = bottom_blob.c;
            top_blob.create(w, h, indices.w, elemsize, opt.blob_allocator);

            if (top_blob.empty())
            {
                return -100;
            }
            for (int i = 0; i < indices.w; i++)
            {
                int                  selected = (int)(indices_ptr[i] + 0.5);
                const unsigned char* ptr      = bottom_blob.channel(selected);
                unsigned char*       outptr   = top_blob.channel(i);

                memcpy(outptr, ptr, w * h * elemsize);
            }
            return 0;
        }

        if (dims == 3 && positive_axis == 1 && indices_dims == 1)
        {
            int w        = bottom_blob.w;
            int h        = bottom_blob.h;
            int channels = bottom_blob.c;
            top_blob.create(w, indices.w, channels, elemsize, opt.blob_allocator);
#pragma omp parallel for num_threads(opt.num_threads)
            // use parallel programming
            for (int i = 0; i < channels; i++)
            {
                float*       outptr = top_blob.channel(i);
                const float* ptr    = bottom_blob.channel(i);
                for (int j = 0; j < indices.w; j++)
                {
                    int selected = (int)(indices_ptr[j] + 0.5);
                    for (int k = 0; k < w; k++)
                    {
                        outptr[j * w + k] = ptr[selected * w + k];
                    }
                }
            }

            return 0;
        }

        if (dims == 3 && positive_axis == 2 && indices_dims == 1)
        {
            int w        = bottom_blob.w;
            int h        = bottom_blob.h;
            int channels = bottom_blob.c;
            top_blob.create(indices.w, h, channels, elemsize, opt.blob_allocator);
#pragma omp parallel for num_threads(opt.num_threads)
            // use parallel programming
            for (int i = 0; i < channels; i++)
            {
                float*       outptr = top_blob.channel(i);
                const float* ptr    = bottom_blob.channel(i);
                for (int j = 0; j < h; j++)
                {
                    for (int k = 0; k < indices.w; k++)
                    {
                        int selected              = (int)(indices_ptr[k] + 0.5);
                        outptr[j * indices.w + k] = ptr[j * w + selected];
                    }
                }
            }
            return 0;
        }

        return 0;
    }

}  //  namespace mmdeploy
