#pragma once

#include "Onyx/Core/Base.h"

#include "Onyx/Scene/Model.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Onyx
{

	struct TransformComponent
	{
		glm::vec3 Translation{ 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale{ 1.0f, 1.0f, 1.0f };
		glm::vec3 Rotation{ 0.0f, 0.0f, 0.0f };

		glm::mat4 GetTransform() const;
		glm::mat3 GetNormalMatrix() const;
	};

	struct PointLightComponent
	{
		float LightIntensity = 1.0f;
	};

	class GameObject
	{
	public:
		using id_t = unsigned int;
		using Map = std::unordered_map<id_t, Ref<GameObject>>;

		GameObject(const GameObject&) = delete; 
		GameObject& operator=(const GameObject&) = delete; 

		GameObject(GameObject&&) = default; 
		GameObject& operator=(GameObject&&) = default;

		GameObject(id_t objId) : m_Id(objId) {}
		~GameObject();

		static Ref<GameObject> Create();
		static Ref<GameObject> CreatePointLight(float intensity = 10.0f, float radius = 0.05f, glm::vec3 color = glm::vec3(1.0f));

		id_t GetId() const { return m_Id; }

	public:
		glm::vec3 Color{};
		TransformComponent Transform{};

		// Optional
		Ref<Model> Model{};
		Scope<PointLightComponent> PointLight = nullptr;

	private:
		id_t m_Id;
	};
}