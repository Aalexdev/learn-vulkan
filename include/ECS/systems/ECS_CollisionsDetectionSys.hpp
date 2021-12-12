#pragma once

#include "ECS/ECS.hpp"
#include "FrameInfo.hpp"
#include "ECS/components/ECS_Transform.hpp"
#include "ECS/components/ECS_Collider.hpp"
#include "physics/collisions/CollisionsPoints.hpp"
#include "physics/collisions/solvers/ImpulseSolver.hpp"

// libs
#include <glm/glm.hpp>

namespace ECS::Systems{
	class CollisionDetection : public System{
		public:
			void update(FrameInfo &info);
		private:
			void resolveCollisions(FrameInfo &info);

			Physics::Collisions::Solvers::ImpulseSolver solver;
	};
}