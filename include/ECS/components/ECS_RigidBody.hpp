#pragma once
// libs
#include <glm/glm.hpp>

namespace ECS{
	namespace Components{
		struct RigidBody{
			glm::vec3 velocity = glm::vec3(0.f);
			glm::vec3 force = glm::vec3(0.f);
			float mass = 1.f;
			float restitution = 1.f;
			float staticFriction = 0.f;
			float dynamicFriction = 0.f;
			bool isDynamic = false;
			bool isKinematic = false;
		};
	}
}