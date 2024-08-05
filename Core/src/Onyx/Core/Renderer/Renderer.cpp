#include "stdafx.h"
#include "Renderer.h"

#include "Onyx/Core/Application.h"

namespace Onyx
{
	Renderer::Renderer()
	{
		RecreateSwapChain();
		CreateCommandBuffers();
	}

	Renderer::~Renderer()
	{
		for (auto& system : m_RenderSystems)
		{
			delete system;
			system = nullptr;
		}

		m_SwapChain = nullptr;

		FreeCommandBuffers();
	}

	void Renderer::Render(FrameInfo& info) const
	{
		for (RenderSystem* system : m_RenderSystems)
			system->Render(info);
	}

	void Renderer::Update(FrameInfo& info, GlobalUbo& ubo) const
	{
		for (RenderSystem* system : m_RenderSystems)
			system->Update(info, ubo);
	}

	VkCommandBuffer Renderer::BeginFrame()
	{
		ONYX_CORE_ASSERT(!m_IsFrameStarted, "Can't call BeginFrame() while already in progress");
		
		VkResult result = m_SwapChain->AcquireNextImage(&m_CurrentImageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			RecreateSwapChain();
			return nullptr;
		}

		ONYX_CORE_ASSERT(result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR, "Failed to acquire swap chain image");
		
		m_IsFrameStarted = true;

		VkCommandBuffer commandBuffer = GetCurrentCommandBuffer();
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		ONYX_VK_ASSERT(vkBeginCommandBuffer(commandBuffer, &beginInfo), "Failed to begin recording command buffer");

		return commandBuffer;
	}

	void Renderer::EndFrame()
	{
		ONYX_CORE_ASSERT(m_IsFrameStarted, "Can't call EndFrame() while frame is not in progress");

		VkCommandBuffer commandBuffer = GetCurrentCommandBuffer();
		ONYX_VK_ASSERT(vkEndCommandBuffer(commandBuffer), "Failed to record command buffer");

		VkResult result = m_SwapChain->SubmitCommandBuffers(&commandBuffer, &m_CurrentImageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || Application::Get().GetWindow().ShouldRebuildSwapChain())
		{
			Application::Get().GetWindow().SetRebuildSwapChain(false);
			RecreateSwapChain();
		} 
		else
		{
			ONYX_CORE_ASSERT(result == VK_SUCCESS, "Failed to present swap chain image!");
		}


		m_IsFrameStarted = false;
		m_CurrentFrameIndex = (m_CurrentFrameIndex + 1) % SwapChain::MAX_FRAMES_IN_FLIGHT;
	}

	void Renderer::BeginSwapChainRenderPass(VkCommandBuffer commandBuffer)
	{
		ONYX_CORE_ASSERT(m_IsFrameStarted, "Can't call BeginSwapChainRenderPass() if frame is not in progress");
		ONYX_CORE_ASSERT(commandBuffer == GetCurrentCommandBuffer(), "Can't begin render pass on command buffer from a different frame");

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_SwapChain->GetRenderPass();
		renderPassInfo.framebuffer = m_SwapChain->GetFrameBuffer(m_CurrentImageIndex);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = m_SwapChain->GetSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.01f, 0.01f, 0.01f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(m_SwapChain->GetSwapChainExtent().width);
		viewport.height = static_cast<float>(m_SwapChain->GetSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor{ {0, 0}, m_SwapChain->GetSwapChainExtent() };

		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
	}

	void Renderer::EndSwapChainRenderPass(VkCommandBuffer commandBuffer)
	{
		ONYX_CORE_ASSERT(m_IsFrameStarted, "Can't call EndSwapChainRenderPass() if frame is not in progress");
		ONYX_CORE_ASSERT(commandBuffer == GetCurrentCommandBuffer(), "Can't end render pass on command buffer from a different frame");

		vkCmdEndRenderPass(commandBuffer);
	}

	void Renderer::CreateCommandBuffers()
	{
		m_CommandBuffers.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = Application::Get().GetCommandPool().GetHandle();
		allocInfo.commandBufferCount = static_cast<uint32_t>(m_CommandBuffers.size());

		ONYX_VK_ASSERT(vkAllocateCommandBuffers(Application::Get().GetDevice().GetHandle(), &allocInfo, m_CommandBuffers.data()), "Failed to allocate command buffers");
	}

	void Renderer::FreeCommandBuffers()
	{
		vkFreeCommandBuffers(Application::Get().GetDevice().GetHandle(), Application::Get().GetCommandPool().GetHandle(), 
			m_CommandBuffers.size(), m_CommandBuffers.data());
		m_CommandBuffers.clear();
	}

	void Renderer::RecreateSwapChain()
	{
		VkExtent2D extent = Application::Get().GetWindow().GetExtent();
		while (extent.width == 0 || extent.height == 0)
		{
			extent = Application::Get().GetWindow().GetExtent();
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(Application::Get().GetDevice().GetHandle());

		if (m_SwapChain)
		{
			Ref<SwapChain> oldSwapChain = std::move(m_SwapChain);
			m_SwapChain = CreateScope<SwapChain>(extent, oldSwapChain);

			ONYX_CORE_ASSERT(oldSwapChain->CompareSwapFormats(*m_SwapChain.get()), "Swap chain image(or depth) format has changed!");
		}
		else
		{
			m_SwapChain = CreateScope<SwapChain>(extent);
		}
	}
}

