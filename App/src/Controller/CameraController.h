#pragma once

#include "Controller/KeyboardMovementController.h"
#include "Onyx/Scene/Camera.h"

namespace Onyx
{

	class CameraController
	{
	public:
		CameraController();
		~CameraController();

		void SetOrthographicProjection(float left, float right, float top, float bottom, float near_, float far_);
		void SetPerspectiveProjection(float fovy, float aspect, float near_, float far_);

		const Camera& GetCamera() const { return m_Camera; }
		Camera& GetCamera() { return m_Camera; }

		void Move(const glm::vec3& dPosition) { m_CameraObject->Transform.Translation += dPosition; }

		void OnUpdate(float ts);

	private:
		KeyboardMovementController m_Controller;
		Camera m_Camera;
		Ref<GameObject> m_CameraObject;
	};
}