#pragma once

// libs
#include "physics/collisions/Collider.hpp"

// std
#include <memory>

namespace ECS{
	namespace Components{
		struct Collider{
			std::shared_ptr<Physics::Collisions::Collider> collider;
		};
	}
}