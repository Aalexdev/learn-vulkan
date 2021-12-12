#include "ECS/systems/ECS_CollisionsDetectionSys.hpp"
#include "physics/collisions/Collision.hpp"
#include "physics/collisions/solvers/ImpulseSolver.hpp"

// std
#include <vector>

namespace ECS::Systems{
	void CollisionDetection::update(FrameInfo &info){
		resolveCollisions(info);
	}

	void CollisionDetection::resolveCollisions(FrameInfo &info){
		std::vector<Physics::Collisions::Collision> collisions;

		for (auto &a : mEntities){
			auto &colliderA = info.coordinator.GetComponent<ECS::Components::Collider>(a).collider;
			if (!colliderA) continue;

			auto &transformA = info.coordinator.GetComponent<ECS::Components::Transform>(a);

			for (auto &b : mEntities){
				if (a == b) break;

				auto &colliderB = info.coordinator.GetComponent<ECS::Components::Collider>(b).collider;
				if (!colliderB) continue;
				auto &transformB = info.coordinator.GetComponent<ECS::Components::Transform>(b);


				Physics::Collisions::CollisionPoints points = colliderA->TestCollision(&transformA, colliderB.get(), &transformB);

				if (points.hasCollsion)
					collisions.push_back({a, b, points});
			}
		}

		solver.Solve(collisions, info);
	}
}