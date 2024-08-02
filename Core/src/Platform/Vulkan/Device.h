#pragma once

#include "Onyx/Core/Base.h"

#include <vulkan/vulkan.h>

// std lib headers
#include <string>
#include <vector>

namespace Onyx 
{
    class Device 
    {
    public:

        Device();
        ~Device();

        ONYX_NON_COPIABLE(Device);

        VkQueue GetGraphicsQueue() { return m_GraphicsQueue; }
        VkQueue GetPresentQueue() { return m_PresentQueue; }

        uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        VkFormat FindSupportedFormat( const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

        // Buffer Helper Functions
        void CreateBuffer( VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        VkCommandBuffer BeginSingleTimeCommands();
        void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
        void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        void CopyBufferToImage( VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount);

        void CreateImageWithInfo(const VkImageCreateInfo& imageInfo, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

    private:
        VULKAN_HANDLE(VkDevice, m_Device);

        VkQueue m_GraphicsQueue;
        VkQueue m_PresentQueue;
    };

}