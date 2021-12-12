#pragma once

#include "physics/collisions/CollisionsPoints.hpp"
#include "ECS/components/ECS_Transform.hpp"

namespace Physics::Collisions{
	struct SphereCollider;
	struct PlaneCollider;
	
	namespace algo{
		CollisionPoints findSphereSphereCollisionsPoints(
					const SphereCollider* a, const ECS::Components::Transform* ta,
					const SphereCollider* b, const ECS::Components::Transform* tb);

		CollisionPoints findSpherePlaneCollisionsPoints(
					const SphereCollider* a, const ECS::Components::Transform* ta,
					const PlaneCollider* b, const ECS::Components::Transform* tb);

		CollisionPoints findPlaneSphereCollisionsPoints(
					const PlaneCollider* a, const ECS::Components::Transform* ta,
					const SphereCollider* b, const ECS::Components::Transform* tb);
	}
}