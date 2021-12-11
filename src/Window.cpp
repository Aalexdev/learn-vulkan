#include "Window.hpp"

// std
#include <stdexcept>

Window::Window(int w, int h, std::string title) : width(w), height(h), title(title){
	initWindow();
}

Window::~Window(){
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Window::initWindow(){
	// init the glfw library
	if (glfwInit() == GLFW_FALSE)
		throw std::runtime_error("failed to init glfw");

	// for not create a gl contexte
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	// create the window
	window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

	// cjeck if the window has been created
	if (!window)
		throw std::runtime_error("failed to create the window");
	
	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, frameBufferResizedCallback);
}

bool Window::shouldClose() const noexcept{
	return glfwWindowShouldClose(window);
}

void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface){
	if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
		throw std::runtime_error("failed to create the window surface");
}

VkExtent2D Window::getExtent() const noexcept{
	return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
}

bool Window::resized() const noexcept{
	return frameBufferResized;
}

void Window::resetWindowResizedFlage() noexcept{
	frameBufferResized = false;
}

void Window::frameBufferResizedCallback(GLFWwindow *window, int width, int height){
	auto w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	w->frameBufferResized = true;
	w->width = width;
	w->height = height;
}