#pragma once

#include "Onyx/Core/Base.h"
#include "Platform/Vulkan/Descriptor/Binding.h"

namespace Onyx
{
    class DescriptorPool {
    public:
        ONYX_NON_COPIABLE(DescriptorPool);

        DescriptorPool(const std::vector<DescriptorBinding>& descriptorBindings, size_t maxSets);
        ~DescriptorPool();

    private:
        VULKAN_HANDLE(VkDescriptorPool, m_DescriptorPool);
    };
}