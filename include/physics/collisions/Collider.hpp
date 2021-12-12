#pragma once


#include "algo.hpp"
#include "physics/collisions/CollisionsPoints.hpp"
#include "ECS/components/ECS_Transform.hpp"

namespace Physics::Collisions{
	struct SphereCollider;
	struct PlaneCollider;
	
	struct Collider {
		virtual CollisionPoints TestCollision(
			const ECS::Components::Transform* transform,
			const Collider* collider,
			const ECS::Components::Transform* colliderTransform) const = 0;
	
		virtual CollisionPoints TestCollision(
			const ECS::Components::Transform* transform,
			const SphereCollider* sphere,
			const ECS::Components::Transform* sphereTransform) const = 0;
	
		virtual CollisionPoints TestCollision(
			const ECS::Components::Transform* transform,
			const PlaneCollider* plane,
			const ECS::Components::Transform* planeTransform) const = 0;
	};
}

