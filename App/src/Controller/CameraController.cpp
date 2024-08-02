#include "CameraController.h"
#include "CameraController.h"
#include "CameraController.h"

namespace Onyx {

	CameraController::CameraController()
		: m_CameraObject(GameObject::Create())
	{}

	CameraController::~CameraController()
	{
		m_CameraObject = nullptr;
	}

	void CameraController::SetOrthographicProjection(float left, float right, float top, float bottom, float near_, float far_)
	{
		m_Camera.SetOrthographicProjection(left, right, top, bottom, near_, far_);
	}

	void CameraController::SetPerspectiveProjection(float fovy, float aspect, float near_, float far_)
	{
		m_Camera.SetPerspectiveProjection(fovy, aspect, near_, far_);
	}

	void CameraController::OnUpdate(float ts)
	{
		m_Controller.MoveInPlaneXZ(ts, m_CameraObject);
		m_Camera.SetViewYXZ(m_CameraObject->Transform.Translation, m_CameraObject->Transform.Rotation);
	}
}