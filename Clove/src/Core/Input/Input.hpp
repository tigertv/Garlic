#pragma once

#include "Core/Input/KeyCodes.hpp"
#include "Core/Input/MouseButtonCodes.hpp"

namespace clv{
	namespace input{
		bool isKeyPressed(Key key);

		bool isMouseButtonPressed(MouseButton button);
		std::pair<int32, int32> getMousePosition();
		int32 getMouseX();
		int32 getMouseY();
	};
}