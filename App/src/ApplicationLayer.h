#pragma once

#include "Onyx.h"

#include "Controller/CameraController.h"

#include <string>

namespace Onyx
{
	class ApplicationLayer : public Layer
	{
	public:
		ApplicationLayer();
		~ApplicationLayer() override;

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(float ts) override;
		void OnEvent(Event& event) override;

	private:
		void LoadGameObjects();

	private:
		Scope<Renderer> m_Renderer;
		Scope<DescriptorSetManager> m_DescriptorSetManager;
		GameObject::Map m_GameObjects;

		Scope<SimpleRenderSystem> m_SimpleRenderSystem;
		Scope<PointLightRenderSystem> m_PointLightRenderSystem;

		std::vector<Scope<Buffer>> m_UniformBuffers;
		CameraController m_CameraController;
	};
}
