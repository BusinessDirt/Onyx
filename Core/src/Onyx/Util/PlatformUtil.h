#pragma once

#include <string>
#include <chrono>

namespace Onyx 
{
	class FileDialogs 
	{
	public:
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
	};

	class Time {
	public:
		static std::chrono::steady_clock::time_point GetTime() { return std::chrono::high_resolution_clock::now(); }
	};
}