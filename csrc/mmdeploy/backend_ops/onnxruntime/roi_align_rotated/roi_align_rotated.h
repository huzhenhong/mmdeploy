// Copyright (c) OpenMMLab. All rights reserved
#ifndef ONNXRUNTIME_ROI_ALIGN_ROTATED_H
#define ONNXRUNTIME_ROI_ALIGN_ROTATED_H

#include <assert.h>
#include <onnxruntime_cxx_api.h>

#include <cmath>
#include <mutex>
#include <string>
#include <vector>

namespace mmdeploy
{
    struct MMCVRoIAlignRotatedKernel
    {
      public:
        MMCVRoIAlignRotatedKernel(Ort::CustomOpApi ort, const OrtKernelInfo* info)
            : ort_(ort)
        {
            aligned_height_ = ort_.KernelInfoGetAttribute<int64_t>(info, "output_height");
            aligned_width_  = ort_.KernelInfoGetAttribute<int64_t>(info, "output_width");
            sampling_ratio_ = ort_.KernelInfoGetAttribute<int64_t>(info, "sampling_ratio");
            spatial_scale_  = ort_.KernelInfoGetAttribute<float>(info, "spatial_scale");
            aligned_        = ort_.KernelInfoGetAttribute<int64_t>(info, "aligned");
            clockwise_      = ort_.KernelInfoGetAttribute<int64_t>(info, "clockwise");
        }

        void Compute(OrtKernelContext* context);

      private:
        Ort::CustomOpApi ort_;
        int              aligned_height_;
        int              aligned_width_;
        float            spatial_scale_;
        int              sampling_ratio_;
        int              aligned_;
        int              clockwise_;
    };

    struct MMCVRoIAlignRotatedCustomOp
        : Ort::CustomOpBase<MMCVRoIAlignRotatedCustomOp, MMCVRoIAlignRotatedKernel>
    {
        void* CreateKernel(Ort::CustomOpApi api, const OrtKernelInfo* info) const
        {
            return new MMCVRoIAlignRotatedKernel(api, info);
        }
        const char* GetName() const
        {
            return "MMCVRoIAlignRotated";
        }

        size_t GetInputTypeCount() const
        {
            return 2;
        }
        ONNXTensorElementDataType GetInputType(size_t) const
        {
            return ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT;
        }

        size_t GetOutputTypeCount() const
        {
            return 1;
        }
        ONNXTensorElementDataType GetOutputType(size_t) const
        {
            return ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT;
        }

        // force cpu
        const char* GetExecutionProviderType() const
        {
            return "CPUExecutionProvider";
        }
    };
}  // namespace mmdeploy

#endif  // ONNXRUNTIME_ROI_ALIGN_ROTATED_H
