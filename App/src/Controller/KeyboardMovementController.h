#pragma once

#include "Onyx/Scene/GameObject.h"

#include "Onyx/Input/Input.h"

namespace Onyx
{
	class KeyboardMovementController
	{
	public:
		struct Mappings
		{
			KeyCode MoveLeft = Key::A;
			KeyCode MoveRight = Key::D;
			KeyCode MoveForward = Key::W;
			KeyCode MoveBackward = Key::S;
			KeyCode MoveUp = Key::E;
			KeyCode MoveDown = Key::Q;
			KeyCode LookLeft = Key::Left;
			KeyCode LookRight = Key::Right;
			KeyCode LookUp = Key::Up;
			KeyCode LookDown = Key::Down;
		};

		KeyboardMovementController() = default;

		void MoveInPlaneXZ(float dt, Ref<GameObject>& gameObject);

	protected:
		Mappings m_Keys{};
		float m_MoveSpeed = 3.0f;
		float m_LookSpeed = 1.5f;
	};
}