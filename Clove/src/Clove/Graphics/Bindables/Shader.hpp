#pragma once

#include "Clove/Graphics/Bindable.hpp"

#include "Clove/Graphics/GraphicsTypes.hpp"
#include "Clove/Graphics/VertexLayout.hpp"

namespace clv::gfx{
	struct ShaderReflectionData{
		VertexLayout vertexBufferLayout;
	};

	class Shader : public Bindable{
		//FUNCTIONS
	public:
		Shader();
		Shader(const Shader& other) = delete;
		Shader(Shader&& other) noexcept;
		Shader& operator=(const Shader& other) = delete;
		Shader& operator=(Shader&& other) noexcept;
		virtual ~Shader();

		virtual ShaderReflectionData getReflectionData() = 0;
	};
}