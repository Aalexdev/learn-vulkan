#pragma once

#include "ECS/ECS.hpp"
#include "Device.hpp"
#include "FrameInfo.hpp"
#include "Pipeline.hpp"

// libs
#include <vulkan/vulkan.h>

// std
#include <memory>

namespace ECS{
    namespace Systems{
        class Renderer : public System{
            public:
                Renderer(Device &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
                ~Renderer();

                Renderer(const Renderer &) = delete;
                Renderer &operator=(const Renderer &) = delete;

                void renderEntitys(FrameInfo &info);
            
            private:
                void createPipelineLayout(VkDescriptorSetLayout globalSetlayout);
                void createPipeline(VkRenderPass renderPass);

                Device &device;

                std::unique_ptr<Pipeline> pipeline;
                VkPipelineLayout pipelineLayout;
        };
    }
}