#include "stdafx.h"
#include "ShaderManager.h"

#include <filesystem>

#include "Onyx/Assets/Shader/ShaderCode.h"

namespace Onyx
{
	namespace Util
	{
		static const std::filesystem::path GetCacheDirectory(const std::filesystem::path& shaderFolder)
		{
			std::filesystem::path cache = shaderFolder / "cache";
			if (!std::filesystem::exists(cache))
				std::filesystem::create_directories(cache);
			return cache;
		}
	}

	ShaderManager::ShaderManager(const std::filesystem::path& sourcePath)
	{
		const std::filesystem::path cachePath = Util::GetCacheDirectory(sourcePath);
		ONYX_CORE_INFO("Shader Folder: {0}", sourcePath.string().c_str());
		ONYX_CORE_INFO("Cache Folder:  {0}", cachePath.string().c_str());

		for (const auto& sourceFilePath : std::filesystem::directory_iterator(sourcePath))
		{
			if (std::filesystem::is_directory(sourceFilePath)) continue;

			std::string filename = sourceFilePath.path().filename().string();
			std::filesystem::path cachedFilePath = cachePath / (filename + ".spv");

			m_Shaders.insert({ filename, ShaderCode(sourceFilePath, cachedFilePath, !std::filesystem::exists(cachedFilePath)) });
		}


		// TODO: recompile if file has been changed
	}

	ShaderManager::~ShaderManager()
	{
	}
}
