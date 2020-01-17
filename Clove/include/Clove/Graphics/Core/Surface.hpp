#pragma once

namespace clv::gfx{
	class Surface{
		//FUNCTIONS
	public:
		virtual ~Surface() = default;

		virtual void setVSync(bool vsync) = 0;
		virtual bool isVsync() const = 0;

		virtual void resizeBuffers(const mth::vec2ui& size) = 0;

		virtual void present() = 0;
	};
}