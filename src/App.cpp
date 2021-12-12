#include "KeyboardMovementController.hpp"
#include "ECS/ECS.hpp"
#include "Camera.hpp"
#include "Buffer.hpp"
#include "App.hpp"

#include "ECS/components/ECS_Model.hpp"
#include "ECS/components/ECS_Transform.hpp"
#include "ECS/components/ECS_RigidBody.hpp"
#include "ECS/components/ECS_Collider.hpp"

#include "ECS/systems/ECS_GravitySys.hpp"
#include "ECS/systems/ECS_RenderSys.hpp"
#include "ECS/systems/ECS_CollisionsDetectionSys.hpp"

// std
#include <stdexcept>
#include <random>
#include <chrono>
#include <cmath>
#include <array>

// libs
#define GLM_FORCE_ZERO_TO_ONE
#define GLM_FORCE_RADIANS

#include <glm/gtc/constants.hpp>
#include <glm/glm.hpp>

struct GlobalUBO{
    glm::mat4 projectionView{1.f};
    glm::vec4 ambientLight{glm::vec3(1.f), 0.02f};
    glm::vec3 lightPositon{-1.f};
    alignas(16) glm::vec4 lightColor{1.f};
};

App::App(){
    globalPool = DescriptorPool::Builder(device)
        .setMaxSets(SwapChain::MAX_FRAMES_IN_FLIGHT)
        .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, SwapChain::MAX_FRAMES_IN_FLIGHT)
        .build();
	
	model = VKE::Model::createModelFromFile(device, "D:\\dev\\Vulkan\\gravity test\\resources\\models\\sphere.obj");
}

App::~App(){}

void App::run(){

    std::vector<std::unique_ptr<Buffer>> uboBuffers(SwapChain::MAX_FRAMES_IN_FLIGHT);

    for (int i=0; i<uboBuffers.size(); i++){
        uboBuffers[i] = std::make_unique<Buffer>(device, sizeof(GlobalUBO), 1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, device.properties.limits.minUniformBufferOffsetAlignment);
        uboBuffers[i]->map();
    }

    auto globalSetLayout = DescriptorSetLayout::Builder(device).addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS).build();
    
    std::vector<VkDescriptorSet> globalDescriptorSets(SwapChain::MAX_FRAMES_IN_FLIGHT);
    for (int i=0; i<globalDescriptorSets.size(); i++){
        auto bufferInfo = uboBuffers[i]->descriptorInfo();
        DescriptorWriter(*globalSetLayout, *globalPool).writeBuffer(0, &bufferInfo).build(globalDescriptorSets[i]);
    }

    coordinator.RegisterComponent<ECS::Components::Model>();
    coordinator.RegisterComponent<ECS::Components::Transform>();
	coordinator.RegisterComponent<ECS::Components::RigidBody>();
	coordinator.RegisterComponent<ECS::Components::Collider>();

    auto renderSystem = coordinator.RegisterSystem<ECS::Systems::Renderer>(device, renderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout());

    ECS::Signature renderSignature;
    renderSignature.set(coordinator.GetComponentType<ECS::Components::Model>());
    renderSignature.set(coordinator.GetComponentType<ECS::Components::Transform>());
    coordinator.SetSystemSignature<ECS::Systems::Renderer>(renderSignature);

	auto gravitySystem = coordinator.RegisterSystem<ECS::Systems::Gravity>();
	ECS::Signature gravitySignature;

	gravitySignature.set(coordinator.GetComponentType<ECS::Components::RigidBody>());
    coordinator.SetSystemSignature<ECS::Systems::Gravity>(gravitySignature);

	auto CollisionsSystem = coordinator.RegisterSystem<ECS::Systems::CollisionDetection>();
	ECS::Signature collisionSignature;

	collisionSignature.set(coordinator.GetComponentType<ECS::Components::RigidBody>());
	collisionSignature.set(coordinator.GetComponentType<ECS::Components::Collider>());
	collisionSignature.set(coordinator.GetComponentType<ECS::Components::Transform>());

    coordinator.SetSystemSignature<ECS::Systems::CollisionDetection>(collisionSignature);

    loadGameObjects();

    Camera camera{};
    GameObject viewerGameObject = GameObject::createGameObject();
    KeyboardMovementController movementController{};

    // camera.setViewDirection(glm::vec3(0.f), glm::vec3(0.5f, 0.f, 1.f));
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto startTime = std::chrono::high_resolution_clock::now();

    // while the window shouldn't close, we update the events of glfw 
    while (!window.shouldClose()){
        glfwPollEvents();

        auto newTime = std::chrono::high_resolution_clock::now();
        float deltaTick = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
        float timeFromStart = std::chrono::duration<float, std::chrono::seconds::period>(startTime - newTime).count();

        currentTime = newTime;

        movementController.moveInPlaneXZ(window.getGLFWwindow(), deltaTick, viewerGameObject);
        camera.setViewYXZ(viewerGameObject.transform.translation, viewerGameObject.transform.rotation);
        
        float aspect = renderer.getAspectRatio();
        camera.setPerspectivProjection(glm::radians(50.f), aspect, 0.1f, 100.f);

		// event
		// if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_))
		if (glfwGetMouseButton(window.getGLFWwindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
			pushEntiy();

        if (auto commandBuffer = renderer.beginFrame()){
            int frameIndex = renderer.getFrameIndex();
            FrameInfo frameInfo{frameIndex, deltaTick, commandBuffer, camera, globalDescriptorSets[frameIndex], coordinator};

            // update
            GlobalUBO ubo{};
            ubo.projectionView = camera.getProjection() * camera.getView();
            // ubo.lightPositon = viewerGameObject.transform.translation;

			gravitySystem->update(frameInfo);
			CollisionsSystem->update(frameInfo);

            // send ubo to the uniform buffer
            uboBuffers[frameIndex]->writeToBuffer(&ubo);
            uboBuffers[frameIndex]->flush();

            // render
            renderer.beginSwapChainRenderPass(commandBuffer);
            renderSystem->renderEntitys(frameInfo);
            renderer.endSwapChainRenderPass(commandBuffer);

            renderer.endFrame();
        }
    }
    vkDeviceWaitIdle(device.device());
}

void App::loadGameObjects(){
	std::shared_ptr<VKE::Model> quadModel = VKE::Model::createModelFromFile(device, "resources\\models\\quad.obj");

	auto quad = coordinator.CreateEntity();
	coordinator.AddComponent(quad, ECS::Components::Model{quadModel});
    coordinator.AddComponent(quad, ECS::Components::RigidBody{});
    coordinator.AddComponent(quad, ECS::Components::Transform{.translation=glm::vec3(0.f, 1.f, 0.f), .scale=glm::vec3(10.f)});
	coordinator.AddComponent(quad, ECS::Components::Collider{});
}

void App::pushEntiy(){

	ECS::Entity entity;
	try {
		entity = coordinator.CreateEntity();
	} catch (const std::exception &){
		return;
	}
    ECS::Components::Transform transform{};
    transform.translation = {0.f, 0.f, 1.5f};
    transform.scale = glm::vec3(0.2f);

	ECS::Components::RigidBody rigidBody{};
	rigidBody.mass = 1.f;
	rigidBody.velocity = glm::vec3(rand() % 3000 / 1000.f + 2.f);
	rigidBody.velocity.y *= -1;
	rigidBody.force = glm::vec3(0.f);
	rigidBody.isDynamic = true;
	
	Physics::Collisions::SphereCollider collider{};
	
	coordinator.AddComponent(entity, ECS::Components::Collider{});
    coordinator.AddComponent(entity, ECS::Components::Model{model});
    coordinator.AddComponent(entity, transform);
    coordinator.AddComponent(entity, rigidBody);
}
