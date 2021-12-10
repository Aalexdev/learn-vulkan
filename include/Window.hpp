#pragma once

// libs
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

// std
#include <string>

class Window{
    public:
        Window(int w, int h, std::string title);
        ~Window();

        /**
         * @brief return if the window should close
         * @return true or false
         */
        bool shouldClose() const noexcept;

        void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
        VkExtent2D getExtent() const noexcept;
        bool resized() const noexcept;
        void resetWindowResizedFlage() noexcept;
        GLFWwindow* getGLFWwindow() const {return window;}
    
    private:
        static void frameBufferResizedCallback(GLFWwindow *window, int width, int height);
        void initWindow();
        int width;
        int height;
        bool frameBufferResized = false;
        std::string title;

        GLFWwindow* window;
};