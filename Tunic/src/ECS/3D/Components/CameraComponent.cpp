#include "Tunic/ECS/3D/Components/CameraComponent.hpp"

#include <Clove/Graphics/Core/Surface.hpp>
#include <Clove/Platform/Core/Window.hpp>

using namespace clv;

namespace tnc::ecs::_3D {
	CameraComponent::CameraComponent(std::shared_ptr<clv::gfx::RenderTarget> renderTarget, const gfx::Viewport& viewport, const ProjectionMode projection)
		: renderTarget(std::move(renderTarget)),
			viewport(viewport) {
		setProjectionMode(projection);
	}

	CameraComponent::CameraComponent(plt::Window& window, const ProjectionMode projection) {
		viewport = { 0, 0, window.getSize().x, window.getSize().y };
		window.onWindowResize.bind(&CameraComponent::updateViewportSize, this);
		renderTarget = window.getSurface()->getRenderTarget();
		setProjectionMode(projection);
	}

	CameraComponent::CameraComponent(const CameraComponent& other) = default;

	CameraComponent::CameraComponent(CameraComponent&& other) noexcept = default;

	CameraComponent& CameraComponent::operator=(const CameraComponent& other) = default;

	CameraComponent& CameraComponent::operator=(CameraComponent&& other) noexcept = default;

	CameraComponent::~CameraComponent() = default;

	const clv::mth::mat4f& CameraComponent::getProjection() const {
		return currentProjection;
	}

	const clv::mth::mat4f& CameraComponent::getView() const {
		return currentView;
	}

	const mth::vec3f& CameraComponent::getFront() const {
		return cameraFront;
	}

	const mth::vec3f& CameraComponent::getUp() const {
		return cameraUp;
	}

	const mth::vec3f& CameraComponent::getRight() const {
		return cameraRight;
	}

	const clv::gfx::Viewport& CameraComponent::getViewport() const {
		return viewport;
	}

	void CameraComponent::setProjectionMode(const ProjectionMode mode) {
		const float width = static_cast<float>(viewport.width);
		const float height = static_cast<float>(viewport.height);
		const float aspect = width / height;
		const float size = 15.0f;

		const float nearPlane = 0.5f;
		const float farPlane = 10000.0f;

		currentProjectionMode = mode;

		switch(currentProjectionMode) {
			case ProjectionMode::orthographic:
				currentProjection = mth::createOrthographicMatrix(-size * aspect, size * aspect, -size, size, nearPlane, farPlane);
				break;

			case ProjectionMode::perspective:
				currentProjection = mth::createPerspectiveMatrix(45.0f, aspect, nearPlane, farPlane);
				break;

			default:
				break;
		}
	}

	ProjectionMode CameraComponent::getProjectionMode() const {
		return currentProjectionMode;
	}

	void CameraComponent::updateViewportSize(const mth::vec2ui& viewportSize) {
		viewport.width = viewportSize.x;
		viewport.height = viewportSize.y;

		setProjectionMode(currentProjectionMode);
	}
}
