#pragma once

#include "Clove/Graphics/Bindables/Shader.hpp"

#include "Graphics/DirectX-11/Bindables/DX11ConstantBuffer.hpp"

#include <wrl.h>
#include <d3dcommon.h>

struct ID3D11VertexShader;
struct ID3D11PixelShader;

namespace clv::gfx{
	class Renderer;

	class DX11VertexShader;
	class DX11PixelShader;

	struct Transform{
		math::Matrix4f model;
		math::Matrix4f view;
		math::Matrix4f projection;
	};

	struct Colour{
		struct{
			float r = 0.0f;
			float g = 0.0f;
			float b = 0.0f;
			float a = 0.0f;
		} faceColours[6];
	};

	class DX11Shader : public Shader{
		//VARIABLES
	private:
		std::unordered_map<ShaderTypes, std::unique_ptr<Bindable>> shaders;
		DX11VertexShader* vertexShader = nullptr;

		DX11VertexConstantBuffer<Transform> vertCB;
		DX11PixelConstantBuffer<Colour> pixelCB;

		Transform vertTransforms;
		Colour pixelColours;

		//FUNCTIONS	
	public:
		DX11Shader() = delete;
		DX11Shader(const DX11Shader& other) = delete;
		DX11Shader(DX11Shader&& other) noexcept;
		DX11Shader& operator=(const DX11Shader& other) = delete;
		DX11Shader& operator=(DX11Shader&& other) noexcept;
		~DX11Shader();

		DX11Shader(Renderer& renderer);

		virtual void bind(Renderer& renderer) override;
		virtual void unbind() override;

		virtual void attachShader(ShaderTypes type, Renderer& renderer) override;

		virtual void setModelMatrix(const math::Matrix4f& model) override;
		virtual void setViewMatrix(const math::Matrix4f& view) override;
		virtual void setProjectionMatrix(const math::Matrix4f& projection) override;

		inline DX11VertexShader& getVertexShader();
	};

	class DX11VertexShader : public Bindable{
		//VARIABLES
	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
		Microsoft::WRL::ComPtr<ID3DBlob> byteCode;

		//FUNCTIONS
	public:
		DX11VertexShader() = delete;
		DX11VertexShader(const DX11VertexShader& other) = delete;
		DX11VertexShader(DX11VertexShader&& other) noexcept;
		DX11VertexShader& operator=(const DX11VertexShader& other) = delete;
		DX11VertexShader& operator=(DX11VertexShader&& other) noexcept;
		~DX11VertexShader();

		DX11VertexShader(const std::wstring& path, Renderer& renderer);

		virtual void bind(Renderer& renderer) override;
		virtual void unbind() override;

		inline ID3DBlob* getByteCode() const;
	};

	class DX11PixelShader : public Bindable{
		//VARIABLES
	private:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
		Microsoft::WRL::ComPtr<ID3DBlob> byteCode;

		//FUNCTIONS
	public:
		DX11PixelShader() = delete;
		DX11PixelShader(const DX11PixelShader& other) = delete;
		DX11PixelShader(DX11PixelShader&& other) noexcept;
		DX11PixelShader& operator=(const DX11PixelShader& other) = delete;
		DX11PixelShader& operator=(DX11PixelShader&& other) noexcept;
		~DX11PixelShader();

		DX11PixelShader(const std::wstring& path, Renderer& renderer);

		virtual void bind(Renderer& renderer) override;
		virtual void unbind() override;

		inline ID3DBlob* getByteCode() const;
	};
}

#include "DX11Shader.inl"