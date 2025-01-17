// Copyright (c) OpenMMLab. All rights reserved.

#include "mmdeploy/core/utils/formatter.h"
#include "mmdeploy/preprocess/transform/transform.h"

namespace mmdeploy::transform
{

    class Collect : public Transform
    {
      public:
        explicit Collect(const Value& args)
        {
            if (!args.contains("keys") || !args["keys"].is_array())
            {
                MMDEPLOY_ERROR("'keys' is missed in arguments, or it is not an array as expected");
                throw_exception(eInvalidArgument);
            }
            if (args.contains("meta_keys") && !args["meta_keys"].is_array())
            {
                MMDEPLOY_ERROR("'meta_keys' has to be an array");
                throw_exception(eInvalidArgument);
            }

            for (auto& v : args["keys"])
            {
                keys_.push_back(v.get<std::string>());
            }
            if (args.contains("meta_keys"))
            {
                for (auto& v : args["meta_keys"])
                {
                    meta_keys_.push_back(v.get<std::string>());
                }
            }
        }

        Result<void> Apply(Value& data) override
        {
            MMDEPLOY_DEBUG("input: {}", data);
            Value::Object output;

            // collect 'ori_img' and 'attribute' from `input`, because those two fields
            // are given by users, not generated by transform ops
            if (data.contains("ori_img"))
            {
                output["ori_img"] = data["ori_img"];
            }
            if (data.contains("attribute"))
            {
                output["attribute"] = data["attribute"];
            }

            for (auto& meta_key : meta_keys_)
            {
                if (data.contains(meta_key))
                {
                    output["img_metas"][meta_key] = data[meta_key];
                }
            }
            for (auto& key : keys_)
            {
                if (!data.contains(key))
                {
                    MMDEPLOY_INFO("missed key '{}' in input", key);
                    return Status(eInvalidArgument);
                }
                else
                {
                    output[key] = data[key];
                }
            }

            data = std::move(output);
            MMDEPLOY_DEBUG("output: {}", data);
            return success();
        }

      private:
        std::vector<std::string> keys_;
        std::vector<std::string> meta_keys_;
    };

    MMDEPLOY_REGISTER_TRANSFORM(Collect);

}  // namespace mmdeploy::transform
