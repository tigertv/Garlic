#pragma once

#include "Core/ECS/System.hpp"

#include "Core/ECS/2D/Components/TransformComponent.hpp"
#include "Core/ECS/2D/Components/SpriteComponent.hpp"

namespace clv::ecs::_2D{
	class RenderSystem : public System<TransformComponent, SpriteComponent>{
		//FUNCTIONS
	public:
		RenderSystem();
		RenderSystem(const RenderSystem& other) = delete;
		RenderSystem(RenderSystem&& other) noexcept;
		RenderSystem& operator=(const RenderSystem& other) = delete;
		RenderSystem& operator=(RenderSystem&& other) noexcept;
		virtual ~RenderSystem();

		virtual void update(utl::DeltaTime deltaTime) override;
	};
}
