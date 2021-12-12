#pragma once

// libs
#include <glm/glm.hpp>

namespace Physics::Collisions{
	struct CollisionPoints{
		glm::vec3 A;
		glm::vec3 B;
		glm::vec3 normal;
		float depth;
		bool hasCollsion;
	};
}