#pragma once

#include "Core/Graphics/GraphicsTypes.hpp"
#include "Core/Graphics/RenderAPI.hpp"

namespace clv::gfx{
	class VertexBuffer;
	class VertexBufferData;
	class IndexBuffer;
	class Shader;
	class VertexLayout;
	class Renderer;
	class Texture;
	template<typename T> class ShaderBufferObject;
}

namespace clv::gfx::BindableFactory{
	std::shared_ptr<VertexBuffer>			createVertexBuffer(const VertexBufferData& bufferData, Shader& shader);
	std::shared_ptr<IndexBuffer>			createIndexBuffer(const std::vector<uint32>& indexData);
	std::shared_ptr<Shader>					createShader(ShaderStyle style);
	std::shared_ptr<Texture>				createTexture(const std::string& filePath, uint32 bindingPoint);
	std::shared_ptr<Texture>				createTexture(void* bufferData, uint32 bindingPoint, const gfx::TextureDescriptor& descriptor);
	std::shared_ptr<Texture>				createTexture(uint32 bindingPoint, const gfx::TextureDescriptor& descriptor);

	template<typename T>
	std::shared_ptr<ShaderBufferObject<T>>	createShaderBufferObject(ShaderType shaderType, uint32 bindingPoint);
	template<typename T>
	std::shared_ptr<ShaderBufferObject<T>>	createShaderBufferObject(ShaderType shaderType, uint32 bindingPoint, T&& data);
};

#include "BindableFactory.inl"