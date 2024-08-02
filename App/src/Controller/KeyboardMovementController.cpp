#include "KeyboardMovementController.h"

namespace Onyx
{
	void KeyboardMovementController::MoveInPlaneXZ(float dt, Ref<GameObject>& gameObject)
	{
		glm::vec3 rotate = glm::vec3(0.0f);
		if (Input::IsKeyPressed(m_Keys.LookRight)) rotate.y += 1.f;
		if (Input::IsKeyPressed(m_Keys.LookLeft)) rotate.y -= 1.f;
		if (Input::IsKeyPressed(m_Keys.LookUp)) rotate.x += 1.f;
		if (Input::IsKeyPressed(m_Keys.LookDown)) rotate.x -= 1.f;

		if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) 
		{
			gameObject->Transform.Rotation += m_LookSpeed * dt * glm::normalize(rotate);
		}

		// limit pitch values between about +/- 85ish degrees
		gameObject->Transform.Rotation.x = glm::clamp(gameObject->Transform.Rotation.x, -1.5f, 1.5f);
		gameObject->Transform.Rotation.y = glm::mod(gameObject->Transform.Rotation.y, glm::two_pi<float>());

		float yaw = gameObject->Transform.Rotation.y;
		const glm::vec3 forwardDir = glm::vec3(std::sin(yaw), 0.0f, std::cos(yaw));
		const glm::vec3 rightDir = glm::vec3(forwardDir.z, 0.0f, -forwardDir.x);
		const glm::vec3 upDir = glm::vec3(0.0f, -1.0f, 0.0f);

		glm::vec3 moveDir = glm::vec3(0.0f);
		if (Input::IsKeyPressed(m_Keys.MoveForward)) moveDir += forwardDir;
		if (Input::IsKeyPressed(m_Keys.MoveBackward)) moveDir -= forwardDir;
		if (Input::IsKeyPressed(m_Keys.MoveRight)) moveDir += rightDir;
		if (Input::IsKeyPressed(m_Keys.MoveLeft)) moveDir -= rightDir;
		if (Input::IsKeyPressed(m_Keys.MoveUp)) moveDir += upDir;
		if (Input::IsKeyPressed(m_Keys.MoveDown)) moveDir -= upDir;

		if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) 
		{
			gameObject->Transform.Translation += m_MoveSpeed * dt * glm::normalize(moveDir);
		}
	}
}
