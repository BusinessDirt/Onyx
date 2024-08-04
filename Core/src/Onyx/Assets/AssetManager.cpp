#include "stdafx.h"
#include "AssetManager.h"

namespace Onyx
{
	AssetManager::AssetManager()
	{
		std::filesystem::path relativePath = "assets";
		m_AssetsPath = std::filesystem::absolute(relativePath);
		m_ShaderManager = CreateScope<ShaderManager>(m_AssetsPath / "shaders");
	}

	AssetManager::~AssetManager()
	{
		m_ShaderManager = nullptr;
	}

	const ShaderCode& AssetManager::GetShader(const std::string& filename) const
	{
		return m_ShaderManager->m_Shaders.at(filename);
	}
}

