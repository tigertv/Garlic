#pragma once

#include "Clove/Graphics/Core/Resources/Texture.hpp"

#include <wrl.h>
#include <d3d11.h>

namespace clv::gfx::d3d{
	class D3DTexture : public Texture{
		//VARIABLES
	private:
		Microsoft::WRL::ComPtr<ID3D11Texture2D> d3dTexture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> d3dResourceView; //TODO: Should probably create an object for resource views
		Microsoft::WRL::ComPtr<ID3D11SamplerState> d3dSampler;

		TextureDescriptor descriptor;

		int32_t BPP = 0;
		
		//FUNCTIONS
	public:
		D3DTexture() = delete;
		D3DTexture(ID3D11Device& d3dDevice, const TextureDescriptor& descriptor, const std::string& pathToTexture);
		D3DTexture(ID3D11Device& d3dDevice, const TextureDescriptor& descriptor, const void* data, int32_t BPP);

		D3DTexture(const D3DTexture& other) = delete;
		D3DTexture(D3DTexture&& other) noexcept;

		D3DTexture& operator=(const D3DTexture& other) = delete;
		D3DTexture& operator=(D3DTexture&& other) noexcept;

		virtual ~D3DTexture();

		const Microsoft::WRL::ComPtr<ID3D11Texture2D>& getD3DTexture() const;
		const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& getD3DShaderResourceView() const;
		const Microsoft::WRL::ComPtr<ID3D11SamplerState>& getD3DSampler() const;

		virtual const TextureDescriptor& getDescriptor() const override;

	private:
		void createTexture(ID3D11Device& d3dDevice, const TextureDescriptor& descriptor, const void* data);
	};
}
