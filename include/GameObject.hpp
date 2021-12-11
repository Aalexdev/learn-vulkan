#pragma once

#include "Model.hpp"

// std
#include <memory>
#include <unordered_map>

// libs
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class GameObject{
	public:
		using id_t = unsigned int;
		using Map = std::unordered_map<id_t, GameObject>;

		struct TransformComponent{
			glm::vec3 translation{};
			glm::vec3 scale{1.f, 1.f, 1.f};
			glm::vec3 rotation{};

			glm::mat4 mat4();
			glm::mat3 normalMatrix();
		};
		
		static GameObject createGameObject();

		GameObject(const GameObject &) = delete;
		GameObject& operator=(const GameObject &) = delete;
		GameObject(GameObject &&) = default;
		GameObject& operator=(GameObject &&) = default;

		id_t getId() {return id;}

		std::shared_ptr<VKE::Model> model;
		glm::vec3 color{};
		TransformComponent transform{};

	private:
		GameObject(id_t objId) : id{objId}{}
		id_t id;
};