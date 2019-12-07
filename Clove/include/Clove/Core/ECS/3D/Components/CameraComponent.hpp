#pragma once

#include "Clove/Core/ECS/Component.hpp"

#include "Clove/Core/Graphics/ShaderBufferTypes.hpp"
#include "Clove/Core/Graphics/GraphicsTypes.hpp"

namespace clv::gfx{
	class RenderTarget;
}

namespace clv::plt{
	class Window;
}

namespace clv::ecs::_3D{
	enum class ProjectionMode{
		orthographic,
		perspective
	};
}

namespace clv::ecs::_3D{
	class CameraComponent : public Component<CameraComponent>{
		friend class RenderSystem;

		//VARIABLES
	private:
		ProjectionMode currentProjectionMode;
		mth::mat4f currentProjection = mth::mat4f(1.0f);

		mth::vec3f cameraUp = mth::vec3f(0.0f, 1.0f, 0.0f);
		mth::vec3f cameraFront = mth::vec3f(0.0f, 0.0f, -1.0f);
		mth::vec3f cameraRight = mth::vec3f(-1.0f, 0.0f, 0.0f);

		gfx::CameraRenderData cameraRenderData;

		std::shared_ptr<gfx::RenderTarget> renderTarget;
		gfx::Viewport viewport;

		//FUNCTIONS
	public:
		CameraComponent(const gfx::Viewport& viewport);

		CameraComponent(const CameraComponent& other) = delete;
		CameraComponent(CameraComponent&& other) noexcept;

		CameraComponent& operator=(const CameraComponent& other) = delete;
		CameraComponent& operator=(CameraComponent&& other) noexcept;

		~CameraComponent();

		const mth::vec3f& getFront() const;
		const mth::vec3f& getUp() const;
		const mth::vec3f& getRight() const;

		void setProjectionMode(ProjectionMode mode);

		void setRenderTarget(const std::shared_ptr<gfx::RenderTarget>& renderTarget);

		void updateViewportSize(const mth::vec2ui& viewportSize);
	};
}