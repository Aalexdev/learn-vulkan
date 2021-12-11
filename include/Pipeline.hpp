#pragma once

// files
#include "Device.hpp"

// libs
#include <vulkan/vulkan.h>

// std
#include <string>
#include <vector>

class Pipeline{
	public:

		struct PipelineConfigInfo {
			PipelineConfigInfo(const PipelineConfigInfo&) = delete;
			PipelineConfigInfo &operator=(const PipelineConfigInfo&) = delete;

			VkPipelineViewportStateCreateInfo viewportInfo;
			VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
			VkPipelineRasterizationStateCreateInfo rasterizationInfo;
			VkPipelineMultisampleStateCreateInfo multisampleInfo;
			VkPipelineColorBlendAttachmentState colorBlendAttachment;
			VkPipelineColorBlendStateCreateInfo colorBlendInfo;
			VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
			std::vector<VkDynamicState> dynamicStateEnables;
			VkPipelineDynamicStateCreateInfo dynamicStateInfo;
			VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
			VkRenderPass renderPass = VK_NULL_HANDLE;
			uint32_t subpass = 0;
		};
		
		Pipeline(const std::string& vertFilepath, const std::string& fragFilepath, Device& device, const PipelineConfigInfo& configInfo);
		~Pipeline();

		// delete copy constructor
		Pipeline(const Pipeline&) = delete;
		Pipeline& operator=(const Pipeline&) = delete;

		static void defaultPipelineConfigInfo(PipelineConfigInfo &configInfo);
		void bind(VkCommandBuffer commandBuffer);
	
	private:
		static std::vector<char> readFile(const std::string &filepath);

		void createGraphicPipeline(const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo);
		void createShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule);

		Device &device;
		VkPipeline graphicPipeline;
		VkShaderModule vertShaderModule;
		VkShaderModule fragShaderModule;
};