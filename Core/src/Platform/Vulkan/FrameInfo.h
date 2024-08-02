#pragma once

#include "Onyx/Core/Base.h"
#include "Onyx/Scene/Camera.h"
#include "Onyx/Scene/GameObject.h"

namespace Onyx
{

#define MAX_LIGHTS 10

	struct PointLight
	{
		glm::vec4 Position{}; // ignore w
		glm::vec4 Color{}; // w is intensity
	};

	struct GlobalUbo
	{
		glm::mat4 View{ 1.0f };
		glm::mat4 Projection{ 1.0f };
		glm::vec4 AmbientLightColor{ 1.0f, 1.0f, 1.0f, 0.02f };
		PointLight PointLights[MAX_LIGHTS];
		int NumLights;
	};

	struct FrameInfo
	{
		int FrameIndex;
		float FrameTime;
		VkCommandBuffer CommandBuffer;
		Camera& Camera;
		VkDescriptorSet GlobalDescriptorSet;
		GameObject::Map& GameObjects;
	};
}