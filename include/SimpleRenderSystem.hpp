// #pragma once

// #include "Pipeline.hpp"
// #include "Device.hpp"
// #include "GameObject.hpp"
// #include "FrameInfo.hpp"

// // std
// #include <memory>

// // libs
// #include <vulkan/vulkan.h>

// class SimpleRenderSystem{
//     public:
//         SimpleRenderSystem(Device &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
//         ~SimpleRenderSystem();

//         SimpleRenderSystem(const SimpleRenderSystem&) = delete;
//         SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;
        
//         void renderGameObjects(FrameInfo &info);
    
//     private:
//         void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
//         void createPipeline(VkRenderPass renderPass);

//         Device &device;

//         std::unique_ptr<Pipeline> pipeline;
//         VkPipelineLayout pipelineLayout;
// };