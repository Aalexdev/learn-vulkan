#pragma once

#include "physics/collisions/Collider.hpp"
#include "algo.hpp"

// libs
#include <glm/glm.hpp>

namespace Physics::Collisions{
	struct PlaneCollider : Collider{
		glm::vec3 plane;
		float distance;

		CollisionPoints TestCollision(
			const ECS::Components::Transform* transform,
			const Collider* collider,
			const ECS::Components::Transform* colliderTransform) const override;
	
		CollisionPoints TestCollision(
			const ECS::Components::Transform* transform,
			const SphereCollider* sphere,
			const ECS::Components::Transform* sphereTransform) const override{
				return algo::findPlaneSphereCollisionsPoints(this, transform, sphere, sphereTransform);
			}
	
		CollisionPoints TestCollision(
			const ECS::Components::Transform* transform,
			const PlaneCollider* plane,
			const ECS::Components::Transform* planeTransform) const override{
				return {};
			}
	};
}