#include "ECS/systems/ECS_GravitySys.hpp"

#include "ECS/components/ECS_RigidBody.hpp"
#include "ECS/components/ECS_Transform.hpp"

namespace ECS::Systems{

void Gravity::update(FrameInfo &info){
	for (auto &entity : mEntities){
		auto &rigidBody = info.coordinator.GetComponent<Components::RigidBody>(entity);
		auto &transform = info.coordinator.GetComponent<Components::Transform>(entity);

		if (!rigidBody.isDynamic) continue;
		rigidBody.force += rigidBody.mass * force;
		rigidBody.velocity += rigidBody.force / rigidBody.mass * info.deltaTime;
		transform.translation += rigidBody.velocity * info.deltaTime;

		rigidBody.force = glm::vec3(0.f);
	}
}

}