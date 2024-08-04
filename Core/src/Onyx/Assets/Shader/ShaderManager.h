#pragma once

#include "Onyx/Core/Base.h"

namespace Onyx
{
	class ShaderCode;

	class ShaderManager
	{
	public:
		ShaderManager(const std::filesystem::path& sourcePath);
		~ShaderManager();

	private:
		std::unordered_map<std::string, ShaderCode> m_Shaders;
		friend class AssetManager;
	};
}