#pragma once

#include "Onyx/Core/Base.h"
#include "Platform/Vulkan/Device.h"

namespace Onyx
{
    class DescriptorPool {
    public:
        class Builder {
        public:
            Builder() = default;

            Builder& AddPoolSize(VkDescriptorType descriptorType, uint32_t count);
            Builder& SetPoolFlags(VkDescriptorPoolCreateFlags flags);
            Builder& SetMaxSets(uint32_t count);

            Scope<DescriptorPool> Build() const;

        private:
            std::vector<VkDescriptorPoolSize> m_PoolSizes{};
            uint32_t m_MaxSets = 1000;
            VkDescriptorPoolCreateFlags m_PoolFlags = 0;
        };

        DescriptorPool(uint32_t maxSets, VkDescriptorPoolCreateFlags poolFlags, const std::vector<VkDescriptorPoolSize>& poolSizes);
        ~DescriptorPool();
        DescriptorPool(const DescriptorPool&) = delete;
        DescriptorPool& operator=(const DescriptorPool&) = delete;

        bool AllocateDescriptor(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const;
        void FreeDescriptors(std::vector<VkDescriptorSet>& descriptors) const;

        void ResetPool();

    private:
        VkDescriptorPool m_DescriptorPool;

        friend class DescriptorWriter;
    };
}