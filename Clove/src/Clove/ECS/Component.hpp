#pragma once

#include "Clove/ECS/ECSTypes.hpp"

namespace clv::ecs{
	class Component{
		//VARIABLES
	public:
		EntityID entityID = INVALID_ENTITY_ID;

		//FUNCTIONS
	public:
		Component();
		Component(const Component& other) = delete;
		Component(Component&& other) noexcept;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) noexcept;
		virtual ~Component();
	};
}