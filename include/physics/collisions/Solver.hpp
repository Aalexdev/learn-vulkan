#pragma once

#include "physics/collisions/Collision.hpp"
#include "FrameInfo.hpp"

// std
#include <vector>

namespace Physics::Collisions{
	class Solver{
		public:
			virtual void Solve(std::vector<Collision> &collisions, FrameInfo &info) const = 0;
	};
};