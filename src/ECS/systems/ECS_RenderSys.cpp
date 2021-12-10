#include "ECS/systems/ECS_RenderSys.hpp"

#include "ECS/components/ECS_Model.hpp"
#include "ECS/components/ECS_Transform.hpp"

// std
#include <vector>
#include <iostream>

namespace ECS{
namespace Systems{

struct SimplePushConstantData{
    glm::mat4 modelMatrix{1.f};
    glm::mat4 normalMatrix{1.f};
};

Renderer::Renderer(Device &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout) : device{device}{
    createPipelineLayout(globalSetLayout);
    createPipeline(renderPass);
}

Renderer::~Renderer(){
    vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
}

void Renderer::createPipelineLayout(VkDescriptorSetLayout globalSetLayout){
    VkPushConstantRange pushConstantRange{};

    pushConstantRange.size = sizeof(SimplePushConstantData);
    pushConstantRange.offset = 0;
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

    std::vector<VkDescriptorSetLayout> descriptorSetLayouts{globalSetLayout};

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
    pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
    pipelineLayoutInfo.flags = 0;

    if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
        throw std::runtime_error("failed to create pipeline layout");
}

void Renderer::createPipeline(VkRenderPass renderPass){
    assert(pipelineLayout != VK_NULL_HANDLE && "Cannot create pipeline before pipeline layout");

    Pipeline::PipelineConfigInfo pipelineConfig{};
    Pipeline::defaultPipelineConfigInfo(pipelineConfig);

    pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout = pipelineLayout;

    pipeline = std::make_unique<Pipeline>("D:\\dev\\Vulkan\\ECS test\\resources\\shaders\\shader.vert.spv", "D:\\dev\\Vulkan\\ECS test\\resources\\shaders\\shader.frag.spv", device, pipelineConfig);
}

void Renderer::renderEntitys(FrameInfo &info){
    pipeline->bind(info.commandBuffer);

    vkCmdBindDescriptorSets(info.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &info.globalDescriptorSet, 0, nullptr);

    for (auto &entity : mEntities){
        auto &model = info.coordinator.GetComponent<Components::Model>(entity);
        if (model.model == nullptr) continue;

        auto &transform = info.coordinator.GetComponent<Components::Transform>(entity);

        SimplePushConstantData data{};
        data.modelMatrix = transform.mat4();
        data.normalMatrix = transform.normalMatrix();

        vkCmdPushConstants(info.commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &data);

        model.model->bind(info.commandBuffer);
        model.model->draw(info.commandBuffer);
    }
}

}
}