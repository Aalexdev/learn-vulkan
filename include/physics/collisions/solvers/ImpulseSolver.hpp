#pragma once

#include "physics/collisions/Solver.hpp"

namespace Physics::Collisions::Solvers{
	class ImpulseSolver : public Solver{
		public:
			void Solve(std::vector<Collision>& collisions, FrameInfo &info) const override;
	};
}