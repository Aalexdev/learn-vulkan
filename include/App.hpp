#pragma once


#include "Window.hpp"
#include "Device.hpp"
#include "Renderer.hpp"
#include "Descriptors.hpp"
#include "ECS/ECS.hpp"
#include "ECS/systems/ECS_RenderSys.hpp"

// std
#include <memory>
#include <unordered_map>

// libs
#include <vulkan/vulkan.h>

class App{
    static constexpr int WIDTH = 720;
    static constexpr int HEIGHT = 720;

    public:
        App();
        ~App();

        App(const App&) = delete;
        App& operator=(const App&) = delete;

        void run();
    
    private:
        void loadGameObjects();

        Window window{WIDTH, HEIGHT, "learn vulkan"};
        Device device{window};
        Renderer renderer{window, device};

        std::unique_ptr<DescriptorPool> globalPool{};
        ECS::Coordinator coordinator{};
};