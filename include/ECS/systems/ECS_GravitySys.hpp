#pragma once

#include "ECS/ECS.hpp"
#include "FrameInfo.hpp"

// libs
#include <glm/glm.hpp>

namespace ECS{
	namespace Systems{
		class Gravity : public System{
			public:
				Gravity(const glm::vec3 force = glm::vec3(0.f, 9.864f, 2.f)) : force{force}{};

				void update(FrameInfo &info);
			
			private:
				glm::vec3 force;
		};
	}
}