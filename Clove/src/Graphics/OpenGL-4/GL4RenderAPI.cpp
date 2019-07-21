#include "clvpch.hpp"
#include "GL4RenderAPI.hpp"

#include "Clove/Graphics/Context.hpp"
#include "Graphics/OpenGL-4/GL4Exception.hpp"
#include "Graphics/OpenGL-4/Bindables/GL4IndexBuffer.hpp"

#include <glad/glad.h>

namespace clv::gfx{
	GL4RenderAPI::~GL4RenderAPI() = default;

	GL4RenderAPI::GL4RenderAPI(const Context& context)
		: RenderAPI(context.getAPI()){
		CLV_ASSERT(gladLoadGL(), "Failed to load OpenGL functions");

		CLV_LOG_TRACE("GL version: {0}", glGetString(GL_VERSION));

		glDepthFunc(GL_LESS); //TODO: Expose to function call

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //TODO: Expose to function call

		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);

		glDebugMessageCallback(errorCallback, nullptr);
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	}

	void GL4RenderAPI::clear(){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GL4RenderAPI::drawIndexed(const unsigned int count){
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(count), GL_UNSIGNED_INT, nullptr);
	}

	void GL4RenderAPI::setClearColour(const math::Vector4f& colour){
		glClearColor(colour.r, colour.g, colour.b, colour.a);
	}
	
	void GL4RenderAPI::setDepthBuffer(bool enabled){
		if(enabled){
			glEnable(GL_DEPTH_TEST);
		} else{
			glDisable(GL_DEPTH_TEST);
		}
	}
	
	void GL4RenderAPI::setBlendState(bool enabled){
		if(enabled){
			glEnable(GL_BLEND);
		} else{
			glDisable(GL_BLEND);
		}
	}
}