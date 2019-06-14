#pragma once

#include "Clove/Graphics/Bindables/Texture.hpp"

namespace clv::gfx{
	class GL4Texture : public Texture{
		//VARIABLES
	private:
		unsigned int rendererID = 0;
		std::string filePath;

		int width = 0;
		int height = 0;
		int BPP = 0;

		unsigned int bindingPoint = 0;

		//FUNCTIONS
	public:
		GL4Texture() = delete;
		GL4Texture(const GL4Texture& other) = delete;
		GL4Texture(GL4Texture&& other) noexcept;
		GL4Texture& operator=(const GL4Texture& other) = delete;
		GL4Texture& operator=(GL4Texture&& other) noexcept;
		virtual ~GL4Texture();

		GL4Texture(const std::string& filePath, unsigned int bindingPoint);

		virtual void bind(Renderer& renderer) override;
		virtual void unbind() override;

		virtual int getWidth() const override;
		virtual int getHeight() const override;
	};
}