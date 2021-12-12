#pragma once

// classes
#include "Device.hpp"
#include "Buffer.hpp"

// libs
#include <vulkan/vulkan.h>

// glm
#define GLM_FORCE_ZERO_TO_ONE
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

// std
#include <memory>
#include <vector>

namespace VKE{
    class Model{
        public:
			static constexpr glm::vec3 MODEL_NON_CUSTOM_COLOR = glm::vec3(2.f);

            struct Vertex{
                glm::vec3 position{};
                glm::vec3 color{};
                glm::vec3 normal{};
                glm::vec2 uv{};

                static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
                static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

                bool operator==(const Vertex &other) const {
                    return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
                }
            };

            struct Builder{
                std::vector<Vertex> vertices{};
                std::vector<uint32_t> indices{};

                void loadModel(const std::string &filepath, glm::vec3 color = MODEL_NON_CUSTOM_COLOR);
            };

            Model(Device &device, const Model::Builder& builder);
            ~Model();

            Model(const Model&) = delete;
            Model &operator=(const Model&) = delete;

            static std::unique_ptr<Model> createModelFromFile(Device &device, const std::string &filepath); 

            void bind(VkCommandBuffer commandBuffer) noexcept;
            void draw(VkCommandBuffer commandBuffer) noexcept;
            
        private:
            void createVertexBuffers(const std::vector<Vertex> &vertices);
            void createIndexBuffers(const std::vector<uint32_t> &indices);

            Device &device;

            std::unique_ptr<Buffer> vertexBuffer;
            uint32_t vertexCount;

            bool hasIndexBuffer {false};
            std::unique_ptr<Buffer> indexBuffer;
            uint32_t indexCount;
    };
}