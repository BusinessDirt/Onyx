#pragma once

#include <vector>

#include "Onyx/Core/Base.h"
#include "Onyx/Core/Renderer/RenderSystem.h"

#include "Platform/Vulkan/SwapChain.h"

#include "Platform/Vulkan/FrameInfo.h"

namespace Onyx
{

	class Renderer
	{
	public:
		Renderer();
		virtual ~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		VkRenderPass GetSwapChainRenderPass() const { return m_SwapChain->GetRenderPass(); }
		float GetAspectRatio() const { return m_SwapChain->ExtentAspectRatio(); }
		bool IsFrameInProgess() const { return m_IsFrameStarted; }
		VkCommandBuffer GetCurrentCommandBuffer() const 
		{ 
			ONYX_CORE_ASSERT(m_IsFrameStarted, "Cannot get command buffer when frame not in progress");
			return m_CommandBuffers[m_CurrentFrameIndex]; 
		}

		int GetFrameIndex() const
		{
			ONYX_CORE_ASSERT(m_IsFrameStarted, "Cannot get command buffer when frame not in progress");
			return m_CurrentFrameIndex;
		}

		void Render(FrameInfo& info) const;
		void Update(FrameInfo& info, GlobalUbo& ubo) const;

	public:
		VkCommandBuffer BeginFrame();
		void EndFrame();
		void BeginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void EndSwapChainRenderPass(VkCommandBuffer commandBuffer);

		void PushRenderSystem(RenderSystem* system)
		{
			m_RenderSystems.emplace_back(system);
		}

	private:
		void CreateCommandBuffers();
		void FreeCommandBuffers();
		void RecreateSwapChain();

	private:
		Scope<SwapChain> m_SwapChain;
		std::vector<VkCommandBuffer> m_CommandBuffers;
		std::vector<RenderSystem*> m_RenderSystems;

		uint32_t m_CurrentImageIndex{ 0 };
		int m_CurrentFrameIndex{ 0 };
		bool m_IsFrameStarted{ false };
	};
}