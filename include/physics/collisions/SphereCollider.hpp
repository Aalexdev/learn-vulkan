#pragma once

#include "physics/collisions/Collider.hpp"
#include "physics/collisions/PlaneCollider.hpp"
#include "physics/collisions/algo.hpp"

// libs
#include <glm/glm.hpp>

namespace Physics::Collisions{
	struct SphereCollider : Collider{
		glm::vec3 center;
		float radius;

		CollisionPoints TestCollision(
			const ECS::Components::Transform* transform,
			const Collider* collider,
			const ECS::Components::Transform* colliderTransform) const override;
	
		CollisionPoints TestCollision(
			const ECS::Components::Transform* transform,
			const SphereCollider* sphere,
			const ECS::Components::Transform* sphereTransform) const override{
				return algo::findSphereSphereCollisionsPoints(this, transform, sphere, sphereTransform);
			}
	
		CollisionPoints TestCollision(
			const ECS::Components::Transform* transform,
			const PlaneCollider* plane,
			const ECS::Components::Transform* planeTransform) const override{
				return algo::findSpherePlaneCollisionsPoints(this, transform, plane, planeTransform);
			}
	};
}