#pragma once

#include <glm/glm.hpp>

#include "Onyx/Core/Base.h"
#include "Onyx/Input/KeyCodes.h"
#include "Onyx/Input/MouseCodes.h"

namespace Onyx 
{
	class Input 
	{
	public:
		static bool IsKeyPressed(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}