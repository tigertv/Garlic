#include "clvpch.hpp"
#include "CameraSystem.hpp"

#include "Clove/Graphics/Renderer.hpp"

namespace clv::ecs{
	CameraSystem::CameraSystem() = default;

	CameraSystem::CameraSystem(CameraSystem&& other) noexcept = default;

	CameraSystem& CameraSystem::operator=(CameraSystem&& other) noexcept = default;

	CameraSystem::~CameraSystem() = default;
	
	void CameraSystem::update(utl::DeltaTime deltaTime){
		for(auto& componentTuple : components){
			Transform3DComponent* transform = std::get<Transform3DComponent*>(componentTuple);
			CameraComponent* camera = std::get<CameraComponent*>(componentTuple);

			const math::Vector3f position = transform->getLocalPosition(); //TODO: How do I get world position?
			//Gotta look up that maths and shit - looks like it is possible (will need to change to quaternions)
			//Just need to understand the math and refactor the function out of how clove handles transforms

			//I think it would make sense to make the world transforms the dominant ones
			//and then calculate the local transforms from those

			//update front
			math::Vector3f front;
			front.x = cos(math::asRadians(camera->yaw)) * cos(math::asRadians(camera->pitch));
			front.y = sin(math::asRadians(camera->pitch));
			front.z = sin(math::asRadians(camera->yaw)) * cos(math::asRadians(camera->pitch));
			camera->cameraFront = math::normalise(front);

			//update look at
			const math::Matrix4f lookAt = math::lookAt(position, position + camera->cameraFront, camera->cameraUp);

			//update right
			camera->cameraRight = math::normalise(math::cross(camera->cameraFront, camera->cameraUp));

			camera->cameraRenderData.lookAt = lookAt;
			camera->cameraRenderData.position = position;
			camera->cameraRenderData.projection = camera->currentProjection;

			//Assuming a single camera. but will just use the last to update if there are multiple
			//will need to change
			gfx::Renderer::setCamera(camera->cameraRenderData);
		}
	}
}