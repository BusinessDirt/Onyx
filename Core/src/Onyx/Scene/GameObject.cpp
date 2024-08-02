#include "stdafx.h"
#include "GameObject.h"

namespace Onyx
{
	glm::mat4 TransformComponent::GetTransform() const
	{
		glm::mat4 rotationMatrix = glm::toMat4(glm::quat(Rotation));

		return glm::translate(glm::mat4(1.0f), Translation)
			* rotationMatrix * glm::scale(glm::mat4(1.0f), Scale);
	}

	glm::mat3 TransformComponent::GetNormalMatrix() const
	{
		return glm::transpose(glm::inverse(glm::mat3(GetTransform())));
	}

	GameObject::~GameObject()
	{
		Model = nullptr;
	}

	Ref<GameObject> GameObject::Create()
	{
		static id_t currentId = 0;
		return CreateRef<GameObject>(currentId++);
	}

	Ref<GameObject> GameObject::CreatePointLight(float intensity, float radius, glm::vec3 color)
	{
		Ref<GameObject> obj = GameObject::Create();
		obj->Color = color;
		obj->Transform.Scale.x = radius;
		obj->PointLight = CreateScope<PointLightComponent>();
		obj->PointLight->LightIntensity = intensity;
		return obj;
	}
}


