// #include "SimpleRenderSystem.hpp"

// // std
// #include <stdexcept>
// #include <array>
// #include <chrono>
// #include <iostream>
// #include <cmath>

// // libs
// // glm
// #define GLM_FORCE_ZERO_TO_ONE
// #define GLM_FORCE_RADIANS
// #include <glm/glm.hpp>

// // the clear color of the window
// #define CLEAR_COLOR {0.01f, 0.01f, 0.01f, 1.f}

// struct SimplePushConstantData{
//     glm::mat4 modelMatrix{1.f};
//     glm::mat4 normalMatrix{1.f};
// };

// SimpleRenderSystem::SimpleRenderSystem(Device &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout) : device{device}{
//     createPipelineLayout(globalSetLayout);
//     createPipeline(renderPass);
// }

// SimpleRenderSystem::~SimpleRenderSystem(){
//     vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
// }

// void SimpleRenderSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout){
//     VkPushConstantRange pushConstantRange{};
//     pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
//     pushConstantRange.offset = 0;
//     pushConstantRange.size = sizeof(SimplePushConstantData);

//     std::vector<VkDescriptorSetLayout> descriptorSetLayouts{globalSetLayout};

//     VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
//     pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
//     pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
//     pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
//     pipelineLayoutInfo.pushConstantRangeCount = 1;
//     pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
//     pipelineLayoutInfo.flags = 0;

//     if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
//         throw std::runtime_error("failed to create pipeline layout");
// }

// void SimpleRenderSystem::createPipeline(VkRenderPass renderPass){
//     assert(pipelineLayout != VK_NULL_HANDLE && "Cannot create pipeline before pipeline layout");

//     Pipeline::PipelineConfigInfo pipelineConfig{};
//     Pipeline::defaultPipelineConfigInfo(pipelineConfig);

//     pipelineConfig.renderPass = renderPass;
//     pipelineConfig.pipelineLayout = pipelineLayout;

//     pipeline = std::make_unique<Pipeline>("resources\\shaders\\shader.vert.spv", "resources\\shaders\\shader.frag.spv", device, pipelineConfig);
// }

// void SimpleRenderSystem::renderGameObjects(FrameInfo &info){
//     pipeline->bind(info.commandBuffer);
	
//     vkCmdBindDescriptorSets(info.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &info.globalDescriptorSet, 0, nullptr);

//     for (auto &kv : info.gameObjects){
//         GameObject &obj = kv.second;

//         // skip non-model objects
//         if (obj.model == nullptr) continue;

//         SimplePushConstantData data{};
//         data.modelMatrix = obj.transform.mat4();
//         data.normalMatrix = obj.transform.normalMatrix();

//         vkCmdPushConstants(info.commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &data);
//         obj.model->bind(info.commandBuffer);
//         obj.model->draw(info.commandBuffer);
//     }
// }