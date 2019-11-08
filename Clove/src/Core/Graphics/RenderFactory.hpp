#pragma once

namespace clv::gfx{
	class IndexBuffer;
	struct IndexBufferDescriptor;
	class ShaderResource;
	struct ShaderResourceDescriptor;
	class VertexBuffer;
	struct VertexBufferDescriptor;
	class Texture;
	struct TextureDescriptor;
	class Shader;
	struct ShaderDescriptor;
	class Surface;
}

namespace clv::gfx{
	class RenderFactory{
		//FUNCTIONS
	public:
		virtual std::shared_ptr<IndexBuffer> createIndexBuffer(const IndexBufferDescriptor& descriptor) = 0;
		virtual std::shared_ptr<VertexBuffer> createVertexBuffer(const VertexBufferDescriptor& descriptor) = 0;

		virtual std::shared_ptr<ShaderResource> createShaderResource(const ShaderResourceDescriptor& descriptor) = 0;

		virtual std::shared_ptr<Texture> createTexture(const TextureDescriptor& descriptor) = 0;

		virtual std::shared_ptr<Shader> createShader(const ShaderDescriptor& descriptor) = 0;

		virtual std::shared_ptr<Surface> createSurface() = 0;
	};
}