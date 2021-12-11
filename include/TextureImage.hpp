#pragma once

#include "Device.hpp"

// std
#include <string>

// libs
#include <vulkan/vulkan.h>

class TextureImage{
	public:
		TextureImage(Device &device, const std::string &filepath);
		~TextureImage();
	
	private:
		void createTextureImage(const std::string &filepath);
		void createImageView();
		void createImageSampler();
		
		static void createImage(Device &device, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage &image, VkDeviceMemory& imageMemory);

		Device &device;

		VkImage textureImage;
		VkImageView textureImageView;
		VkSampler textureImageSampler;
		VkDeviceMemory textureImageMemory;
};