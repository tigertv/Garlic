#include "clvpch.hpp"

#define	IMGUI_IMPL_OPENGL_LOADER_GLAD
#include "examples/imgui_impl_opengl3.cpp"
#if CLV_PLATFORM_WINDOWS
#include "examples/imgui_impl_dx11.cpp"
#include "examples/imgui_impl_win32.cpp"
#endif