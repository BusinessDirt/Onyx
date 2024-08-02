#pragma once

#include "Onyx/Core/Base.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace Onyx
{
	class Log
	{
	public:
		static void Init();

		static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};
}

template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector) 
{
	return os << glm::to_string(vector);
}

template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix) 
{
	return os << glm::to_string(matrix);
}

template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion) 
{
	return os << glm::to_string(quaternion);
}

#ifndef ONYX_DIST

// Core log macros
#define ONYX_CORE_TRACE(...)     ::Onyx::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define ONYX_CORE_INFO(...)      ::Onyx::Log::GetCoreLogger()->info(__VA_ARGS__)
#define ONYX_CORE_WARN(...)      ::Onyx::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define ONYX_CORE_ERROR(...)     ::Onyx::Log::GetCoreLogger()->error(__VA_ARGS__)
#define ONYX_CORE_CRITICAL(...)     ::Onyx::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define ONYX_TRACE(...)   ::Onyx::Log::GetClientLogger()->trace(__VA_ARGS__)
#define ONYX_INFO(...)    ::Onyx::Log::GetClientLogger()->info(__VA_ARGS__)
#define ONYX_WARN(...)    ::Onyx::Log::GetClientLogger()->warn(__VA_ARGS__)
#define ONYX_ERROR(...)   ::Onyx::Log::GetClientLogger()->error(__VA_ARGS__)
#define ONYX_CRITICAL(...)   ::Onyx::Log::GetClientLogger()->critical(__VA_ARGS__)

#else // strip logging in dist build

// Core log macros
#define ONYX_CORE_TRACE(...)
#define ONYX_CORE_INFO(...)
#define ONYX_CORE_WARN(...)
#define ONYX_CORE_ERROR(...)
#define ONYX_CORE_FATAL(...)

// Client log macros
#define ONYX_TRACE(...)
#define ONYX_INFO(...)
#define ONYX_WARN(...)
#define ONYX_ERROR(...)
#define ONYX_FATAL(...)

#endif