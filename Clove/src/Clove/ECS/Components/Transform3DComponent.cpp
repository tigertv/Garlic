#include "clvpch.hpp"
#include "Transform3DComponent.hpp"

namespace clv::ecs{
	Transform3DComponent::Transform3DComponent() = default;

	Transform3DComponent::Transform3DComponent(Transform3DComponent&& other) noexcept = default;

	Transform3DComponent& Transform3DComponent::operator=(Transform3DComponent&& other) noexcept = default;

	Transform3DComponent::~Transform3DComponent(){
		if(parent){
			auto removeIter = std::remove(parent->children.begin(), parent->children.end(), this);
			parent->children.erase(removeIter, parent->children.end());
		}

		for(auto* child : children){
			child->parent = nullptr;
		}
	}

	const math::Vector3f& Transform3DComponent::getPosition() const{
		return position;
	}

	const math::Vector3f& Transform3DComponent::getLocalPosition() const{
		return localPosition;
	}
	
	const math::Quaternionf& Transform3DComponent::getRotation() const{
		return rotation;
	}

	const math::Quaternionf& Transform3DComponent::getLocalRotation() const{
		return localRotation;
	}
	
	const math::Vector3f& Transform3DComponent::getScale() const{
		return scale;
	}

	const math::Vector3f& Transform3DComponent::getLocalScale() const{
		return localScale;
	}
	
	void Transform3DComponent::setPosition(const math::Vector3f& position){
		desiredPosition = position;
	}

	void Transform3DComponent::setLocalPosition(const math::Vector3f& position){
		desiredLocalPosition = position;
	}

	void Transform3DComponent::setRotation(const math::Quaternionf& rotation){
		desiredRotation = rotation;
	}

	void Transform3DComponent::setLocalRotation(const math::Quaternionf& rotation){
		desiredLocalRotation = rotation;
	}

	void Transform3DComponent::setScale(const math::Vector3f& scale){
		desiredScale = scale;
	}

	void Transform3DComponent::setLocalScale(const math::Vector3f& scale){
		desiredLocalScale = scale;
	}

	Transform3DComponent* Transform3DComponent::getParent() const{
		return parent;
	}

	void Transform3DComponent::addChild(Transform3DComponent* child){
		if(child && child != this){
			children.push_back(child);
			child->parent = this;
		}
	}
	
	const math::Matrix4f& Transform3DComponent::getWorldTransformMatrix() const{
		return worldTransformMatrix;
	}

	const math::Matrix4f& Transform3DComponent::getLocalTransformMatrix() const{
		return localTransformMatrix;
	}
}