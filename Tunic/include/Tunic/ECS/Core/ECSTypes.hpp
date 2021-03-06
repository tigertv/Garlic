#pragma once

namespace tnc::ecs{
	using EntityID		= uint32_t;
	using ComponentID	= uint64_t;

	//static constexpr uint64_t	INVALID_POOL_INDEX	= std::numeric_limits<uint64_t>::max();
	static constexpr EntityID	INVALID_ENTITY_ID	= 0;
}