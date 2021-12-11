#pragma once

#include "Device.hpp"

// libs
#include <vulkan/vulkan.h>

class SingleTimeCommandBuffer{
    public:
        SingleTimeCommandBuffer(Device &device);
        ~SingleTimeCommandBuffer();

        static void copyBuffer(Device &device, VkBuffer src, VkBuffer dst, VkDeviceSize size);
        static void transitionImageLayout(Device &device, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
        static void copyBufferToImage(Device &device, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

        VkCommandBuffer getCommandBuffer() const {return commandBuffer;};
    
    private:
        void begin();
        void end();

        VkCommandBuffer commandBuffer;
        Device &device;
};