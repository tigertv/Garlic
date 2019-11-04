#pragma once

#include "Core/ECS/ECSTypes.hpp"
#include "Core/Utils/DeltaTime.hpp"

namespace clv::ecs{
	class Component;

	class SystemBase{
		//FUNCTIONS
	public:
		virtual ~SystemBase() = default;

		virtual void onEntityComponentAdded(EntityID entity, const std::unordered_map<ComponentID, std::unique_ptr<Component>>& entityComponents) = 0;
		virtual void onEntityDestroyed(EntityID entity) = 0;

		virtual void update(utl::DeltaTime deltaTime) = 0;
	};

	template<typename ...ComponentTypes>
	class System : public SystemBase{
	protected:
		using ComponentTuple = std::tuple<std::add_pointer_t<ComponentTypes>...>;
		using EntityIDToIndexMap = std::unordered_map<EntityID, size_t, std::hash<EntityID>, std::equal_to<EntityID>/*, PooledAllocator*/>;

		//VARIABLES
	protected:
		std::vector<ComponentTuple/*, PooledAllocator*/> components; //TODO: using default allocator but we'd want a custom one to allign memory for iterating

	private:
		EntityIDToIndexMap entityIdToIndexMap;

		//FUNCTIONS
	public:
		System();
		System(const System& other) = delete;
		System(System&& other) noexcept;
		System& operator=(const System& other) = delete;
		System& operator=(System&& other) noexcept;
		virtual ~System();

		virtual void onEntityComponentAdded(EntityID entity, const std::unordered_map<ComponentID, std::unique_ptr<Component>>& entityComponents) override final;
		virtual void onEntityDestroyed(EntityID entity) override final;

	private:
		//TODO: Would prefer these to be pure virtual but optionally overriden
		virtual void handleEntityCreation(const ComponentTuple& componentTuple){}
		virtual void handleEntityDestruction(const ComponentTuple& componentTuple){}

		template<size_t index, typename ComponentType, typename ...ComponentArgs>
		bool proccessEntityComponent(ComponentID componentID, Component* component, ComponentTuple& tupleToFill);
		template<size_t index>
		bool proccessEntityComponent(ComponentID componentID, Component* component, ComponentTuple& tupleToFill);
	};
}

#include "System.inl"