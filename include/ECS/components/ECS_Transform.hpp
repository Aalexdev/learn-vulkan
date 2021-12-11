#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ECS{
    namespace Components{

        struct Transform{
            glm::vec3 translation{};
            glm::vec3 scale{1.f, 1.f, 1.f};
            glm::vec3 rotation{};

            glm::mat4 mat4();
            glm::mat3 normalMatrix();
        };

    } // namespace Components
} // namespace ECS