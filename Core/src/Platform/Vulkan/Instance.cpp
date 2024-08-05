#include "stdafx.h"
#include "Instance.h"

#include <string>
#include <unordered_set>

#include "Platform/Vulkan/DebugMessenger.h"

namespace Onyx
{
	Instance::Instance()
	{
#ifdef ONYX_DEBUG
        ONYX_CORE_ASSERT(CheckValidationLayerSupport(), "Validation layers requested, but not available!")
#endif

        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Onyx App";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_3;

        VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        auto extensions = GetRequiredExtensions();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

#ifdef ONYX_DEBUG
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
        createInfo.enabledLayerCount = static_cast<uint32_t>(m_ValidationLayers.size());
        createInfo.ppEnabledLayerNames = m_ValidationLayers.data();

        DebugMessenger::PopulateCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
#else
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
#endif

        ONYX_VK_ASSERT(vkCreateInstance(&createInfo, nullptr, &m_Instance), "Failed to create Instance!");

        HasRequiredExtensions();
	}

	Instance::~Instance()
	{
		vkDestroyInstance(m_Instance, nullptr);
	}

    bool Instance::CheckValidationLayerSupport()
    {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : m_ValidationLayers)
        {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers)
            {
                if (strcmp(layerName, layerProperties.layerName) == 0)
                {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) return false;
        }

        return true;
    }

    std::vector<const char*> Instance::GetRequiredExtensions()
    {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

#ifdef ONYX_DEBUG
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

        return extensions;
    }

    void Instance::HasRequiredExtensions()
    {
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

        ONYX_CORE_INFO("Available extensions: ");
        std::unordered_set<std::string> available;
        for (const auto& extension : extensions)
        {
            ONYX_CORE_INFO("\t{0}", extension.extensionName);
            available.insert(extension.extensionName);
        }

        ONYX_CORE_INFO("Required extensions:");
        auto requiredExtensions = GetRequiredExtensions();
        for (const auto& required : requiredExtensions)
        {
            ONYX_CORE_INFO("\t{0}", required);
            ONYX_CORE_ASSERT(available.find(required) != available.end(), "Missing required glfw extension");
        }
    }
}
