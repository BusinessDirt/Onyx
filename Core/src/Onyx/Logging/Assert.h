#pragma once

#include "Onyx/Core/Base.h"
#include "Onyx/Logging/Log.h"

#include <filesystem>

#ifdef ONYX_ENABLE_ASSERTS

	// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
	// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
	#define ONYX_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { ONYX##type##ERROR(msg, __VA_ARGS__); ONYX_DEBUGBREAK(); } }
	#define ONYX_INTERNAL_ASSERT_WITH_MSG(type, check, ...) ONYX_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
	#define ONYX_INTERNAL_ASSERT_NO_MSG(type, check) ONYX_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", ONYX_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

	#define ONYX_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define ONYX_INTERNAL_ASSERT_GET_MACRO(...) ONYX_EXPAND_MACRO( ONYX_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, ONYX_INTERNAL_ASSERT_WITH_MSG, ONYX_INTERNAL_ASSERT_NO_MSG) )

	// Currently accepts at least the condition and one additional parameter (the message) being optional
	#define ONYX_ASSERT(...) ONYX_EXPAND_MACRO( ONYX_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
	#define ONYX_CORE_ASSERT(...) ONYX_EXPAND_MACRO( ONYX_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )

	// Vulkan creation asserts
	#define ONYX_VK_ASSERT(result, message) ONYX_CORE_ASSERT(result == VK_SUCCESS, message)

#else

	#define ONYX_ASSERT(...)
	#define ONYX_CORE_ASSERT(...)
	#define ONYX_VK_ASSERT(result, message) result

#endif