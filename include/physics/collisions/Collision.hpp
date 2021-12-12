#pragma once

#include "physics/collisions/CollisionsPoints.hpp"
#include "ECS/ECS.hpp"

namespace Physics::Collisions{
	struct Collision{
		const ECS::Entity& A;
		const ECS::Entity& B;
		Physics::Collisions::CollisionPoints points;
	};
}