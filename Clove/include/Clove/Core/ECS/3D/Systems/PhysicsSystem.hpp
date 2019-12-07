#pragma once

#include "Clove/Core/ECS/System.hpp"

class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;

namespace clv::ecs::_3D{
	class PhysicsSystem : public System{
		//VARIABLES
	private:
		btDefaultCollisionConfiguration* collisionConfiguration;
		btCollisionDispatcher* dispatcher;
		btBroadphaseInterface* overlappingPairCache;
		btSequentialImpulseConstraintSolver* solver;

		btDiscreteDynamicsWorld* dynamicsWorld;

		//FUNCTIONS
	public:
		PhysicsSystem();
		PhysicsSystem(const PhysicsSystem& other) = delete;
		PhysicsSystem(PhysicsSystem&& other) noexcept;
		PhysicsSystem& operator=(const PhysicsSystem& other) = delete;
		PhysicsSystem& operator=(PhysicsSystem&& other) noexcept;
		virtual ~PhysicsSystem();

		virtual void update(utl::DeltaTime deltaTime) override;

		virtual void onComponentCreated(ComponentInterface* component) override;
		virtual void onComponentDestroyed(ComponentInterface* component) override;
	};
}