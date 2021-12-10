#pragma once

#include "Camera.hpp"
#include "GameObject.hpp"
#include "ECS/ECS.hpp"

// lib
#include <vulkan/vulkan.h>

struct FrameInfo{
    int frameIndex;
    float deltaTime;
    VkCommandBuffer commandBuffer;
    Camera &camera;
    VkDescriptorSet globalDescriptorSet;
    ECS::Coordinator &coordinator;
};