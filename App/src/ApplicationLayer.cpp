#include "ApplicationLayer.h"

#include "RenderSystems/PointLightRenderSystem.h"
#include "RenderSystems/SimpleRenderSystem.h"

namespace Onyx
{

	ApplicationLayer::ApplicationLayer()
		: Layer("Application Layer")
	{
		std::vector<DescriptorBinding> descriptorBindings =
		{
			{0, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS }
		};

		m_DescriptorSetManager.reset(new DescriptorSetManager(descriptorBindings, SwapChain::MAX_FRAMES_IN_FLIGHT));

		LoadGameObjects();

		m_CameraController.Move({ 0.0f, 0.0f, -2.5f });
	}

	ApplicationLayer::~ApplicationLayer()
	{}

	void ApplicationLayer::OnAttach()
	{
		m_UniformBuffers.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);
		for (int i = 0; i < m_UniformBuffers.size(); i++)
		{
			m_UniformBuffers[i] = CreateScope<Buffer>(sizeof(GlobalUbo), 1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
			m_UniformBuffers[i]->Map();
		}

		DescriptorSets& descriptorSets = m_DescriptorSetManager->GetSets();
		for (int i = 0; i < SwapChain::MAX_FRAMES_IN_FLIGHT; i++)
		{
			VkDescriptorBufferInfo uniformBufferInfo = m_UniformBuffers[i]->GetDescriptorInfo();
			
			// materials, textures etc

			const std::vector<VkWriteDescriptorSet> descriptorWrites =
			{
				descriptorSets.Bind(i, 0, uniformBufferInfo)
			};

			descriptorSets.Update(i, descriptorWrites);
		}

		Application::Get().GetRenderer().PushRenderSystem(new SimpleRenderSystem(m_DescriptorSetManager->GetLayout()));
		Application::Get().GetRenderer().PushRenderSystem(new PointLightRenderSystem(m_DescriptorSetManager->GetLayout()));
	}

	void ApplicationLayer::OnDetach()
	{
		for (auto& [key, obj] : m_GameObjects)
		{
			obj = nullptr;
		}

		for (int i = 0; i < m_UniformBuffers.size(); i++)
		{
			m_UniformBuffers[i] = nullptr;
		}

		m_DescriptorSetManager = nullptr;
	}

	void ApplicationLayer::OnUpdate(float ts)
	{
		m_CameraController.OnUpdate(ts);
		m_CameraController.SetPerspectiveProjection(glm::radians(50.0f), Application::Get().GetRenderer().GetAspectRatio(), 0.1f, 100.0f);

		if (VkCommandBuffer commandBuffer = Application::Get().GetRenderer().BeginFrame())
		{
			int frameIndex = Application::Get().GetRenderer().GetFrameIndex();
			FrameInfo frameInfo{ frameIndex, ts, commandBuffer, m_CameraController.GetCamera(), m_DescriptorSetManager->GetSets().GetHandle(frameIndex), m_GameObjects};

			// update
			GlobalUbo ubo{};
			ubo.View = m_CameraController.GetCamera().GetView();
			ubo.Projection = m_CameraController.GetCamera().GetProjection();

			Application::Get().GetRenderer().Update(frameInfo, ubo);

			m_UniformBuffers[frameIndex]->WriteToBuffer(&ubo);

			// render
			Application::Get().GetRenderer().BeginSwapChainRenderPass(commandBuffer);
			Application::Get().GetRenderer().Render(frameInfo);
			Application::Get().GetRenderer().EndSwapChainRenderPass(commandBuffer);
			Application::Get().GetRenderer().EndFrame();
		}
	}

	void ApplicationLayer::OnEvent(Event& event)
	{}

	void ApplicationLayer::LoadGameObjects()
	{
		std::shared_ptr<Model> model = Model::Create("assets/models/flat_vase.obj");

		Ref<GameObject> flatVase = GameObject::Create();
		flatVase->Model = model;
		flatVase->Transform.Translation = glm::vec3(-0.5f, 0.5f, 0.0f);
		flatVase->Transform.Scale = glm::vec3(3.0f, 1.5f, 3.0f);
		m_GameObjects.emplace(flatVase->GetId(), std::move(flatVase));

		model = Model::Create("assets/models/smooth_vase.obj");

		Ref<GameObject> smoothVase = GameObject::Create();
		smoothVase->Model = model;
		smoothVase->Transform.Translation = glm::vec3(0.5f, 0.5f, 0.0f);
		smoothVase->Transform.Scale = glm::vec3(3.0f, 1.5f, 3.0f);
		m_GameObjects.emplace(smoothVase->GetId(), std::move(smoothVase));

		model = Model::Create("assets/models/quad.obj");

		Ref<GameObject> floor = GameObject::Create();
		floor->Model = model;
		floor->Transform.Translation = glm::vec3(0.0f, 0.5f, 0.0f);
		floor->Transform.Scale = glm::vec3(3.0f, 1.0f, 3.0f);
		m_GameObjects.emplace(floor->GetId(), std::move(floor));

		std::vector<glm::vec3> lightColors{
			{ 1.0f, 0.1f, 0.1f },
			{ 0.1f, 0.1f, 1.0f },
			{ 0.1f, 1.0f, 0.1f },
			{ 1.0f, 1.0f, 0.1f },
			{ 0.1f, 1.0f, 1.0f },
			{ 1.0f, 1.0f, 1.0f }
		};

		for (int i = 0; i < lightColors.size(); i++)
		{
			Ref<GameObject> pointLight = GameObject::CreatePointLight(0.2f);
			pointLight->Color = lightColors[i];

			glm::mat4 rotateLight = glm::rotate(glm::mat4(1.0f), (i * glm::two_pi<float>()) / lightColors.size(), glm::vec3(0.0f, -1.0f, 0.0f));
			pointLight->Transform.Translation = glm::vec3(rotateLight * glm::vec4(-1.0f, -1.0f, -1.0f, -1.0f));

			m_GameObjects.emplace(pointLight->GetId(), std::move(pointLight));
		}
	}
}
