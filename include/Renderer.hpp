#pragma once


#include "Window.hpp"
#include "Device.hpp"
#include "SwapChain.hpp"

// std
#include <memory>
#include <vector>
#include <cassert>

// libs
#include <vulkan/vulkan.h>

class Renderer{
    public:
        Renderer(Window &window, Device &device);
        ~Renderer();

        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;
        
        VkRenderPass getSwapChainRenderPass() const {return swapChain->getRenderPass();}
        float getAspectRatio() const {return swapChain->extentAspectRatio();}
        bool isFrameInProgress() const {return isFrameStarted;}

        VkCommandBuffer getCurrentCommandBuffer() const {
            assert(isFrameStarted && "Cannot get command buffer whene frame not in progress");
            return commandBuffers[currentFrameIndex];
        };
        
        int getFrameIndex() const {
            assert(isFrameStarted && "Cannot get frame index whene frame not in progress");
            return currentFrameIndex;
        }


        VkCommandBuffer beginFrame();
        void endFrame();
        void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
        void endSwapChainRenderPass(VkCommandBuffer commandBuffer);
    
    private:
        void createCommandBuffers();
        void freeCommandBuffers();
        void recreateSwapChain();

        Window &window;
        Device &device;
        std::unique_ptr<SwapChain> swapChain;
        std::vector<VkCommandBuffer> commandBuffers{};

        uint32_t currentImageIndex{0};
        int currentFrameIndex{0};
        bool isFrameStarted{false};
};