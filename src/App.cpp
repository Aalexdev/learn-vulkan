#include "KeyboardMovementController.hpp"
#include "ECS/ECS.hpp"
#include "Camera.hpp"
#include "Buffer.hpp"
#include "App.hpp"

#include "ECS/components/ECS_Model.hpp"
#include "ECS/components/ECS_Transform.hpp"

// std
#include <stdexcept>
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
    glm::vec4 ambientLight{glm::vec3(1.f), .02f};
    glm::vec3 lightPositon{-1.f};
    alignas(16) glm::vec4 lightColor{1.f};
};

App::App(){
    globalPool = DescriptorPool::Builder(device)
        .setMaxSets(SwapChain::MAX_FRAMES_IN_FLIGHT)
        .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, SwapChain::MAX_FRAMES_IN_FLIGHT)
        .build();
    
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

    auto renderSystem = coordinator.RegisterSystem<ECS::Systems::Renderer>(device, renderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout());

    ECS::Signature signature;
    signature.set(coordinator.GetComponentType<ECS::Components::Model>());
    signature.set(coordinator.GetComponentType<ECS::Components::Transform>());
    coordinator.SetSystemSignature<ECS::Systems::Renderer>(signature);

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

        if (auto commandBuffer = renderer.beginFrame()){
            int frameIndex = renderer.getFrameIndex();
            FrameInfo frameInfo{frameIndex, deltaTick, commandBuffer, camera, globalDescriptorSets[frameIndex], coordinator};

            // update
            GlobalUBO ubo{};
            ubo.projectionView = camera.getProjection() * camera.getView();
            // ubo.lightPositon = viewerGameObject.transform.translation;

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
    std::shared_ptr<VKE::Model> model = VKE::Model::createModelFromFile(device, "D:\\dev\\Vulkan\\ECS test\\resources\\models\\smooth_vase.obj");

    auto entity = coordinator.CreateEntity();
    ECS::Components::Transform transform{};
    transform.translation = {0.f, 0.f, .5f};
    transform.scale = {3.f, 1.5f, 3.f};

    coordinator.AddComponent(entity, ECS::Components::Model{model});
    coordinator.AddComponent(entity, transform);

    // GameObject object = GameObject::createGameObject();
    // object.model = model;
    // object.transform.translation = {0.f, 0.f, .5f};
    // object.transform.scale = {3.f, 1.5f, 3.f};
    // gameObjects.emplace(object.getId(), std::move(object));

    // GameObject object2 = GameObject::createGameObject();
    // object2.model = model;
    // object2.transform.translation = {0.f, 0.f, 1.5f};
    // object2.transform.scale = {3.f, 1.5f, 3.f};
    // gameObjects.emplace(object2.getId(), std::move(object2));

    // std::shared_ptr<VKE::Model> quadModel = VKE::Model::createModelFromFile(device, "D:\\dev\\Vulkan\\ECS test\\resources\\models\\quad.obj");
    // GameObject quad = GameObject::createGameObject();
    // quad.model = quadModel;
    // quad.transform.translation = {0.f, 0.5f, 0.f};
    // quad.transform.scale = {10.5f, 10.5f, 10.5f};
    // gameObjects.emplace(quad.getId(), std::move(quad));
}