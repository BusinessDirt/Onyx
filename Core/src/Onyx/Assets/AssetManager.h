#pragma once

#include "Onyx/Core/Base.h"

#include "Onyx/Assets/Shader/ShaderManager.h"
#include "Onyx/Assets/Shader/ShaderCode.h"

namespace Onyx
{
	class AssetManager
	{
	public:
		AssetManager();
		~AssetManager();

		const ShaderCode& GetShader(const std::string& filename) const;

	private:
		std::filesystem::path m_AssetsPath;
		Scope<ShaderManager> m_ShaderManager;
	};
}