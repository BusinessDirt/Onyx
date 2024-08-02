#pragma once

#include <memory>
#include <vulkan/vulkan.h>

#include "Onyx/Core/PlatformDetection.h"

#ifdef ONYX_DEBUG
	#if defined(ONYX_PLATFORM_WINDOWS)
		#define ONYX_DEBUGBREAK() __debugbreak()
	#elif defined(ONYX_PLATFORM_LINUX)
		#include <signal.h>
		#define ONYX_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif
	#define ONYX_ENABLE_ASSERTS
#else
	#define ONYX_DEBUGBREAK()
#endif

#define ONYX_EXPAND_MACRO(x) x
#define ONYX_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)
#define ONYX_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#define ONYX_NON_COPIABLE(name) name(const name&) = delete; name& operator=(const name&) = delete; name(name&&) = delete; name& operator=(name&&) = delete
#define VULKAN_HANDLE(VulkanHandleType, name) public: VulkanHandleType GetHandle() const { return name; } private: VulkanHandleType name = VK_NULL_HANDLE

namespace Onyx {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args) 
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args) 
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}

#include "Onyx/Logging/Log.h"
#include "Onyx/Logging/Assert.h"