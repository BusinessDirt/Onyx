#pragma once

#include "Platform/Vulkan/Device.h"
#include "Platform/Vulkan/Buffer/Buffer.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>

namespace Onyx
{
	class Model
	{
	public:

		struct Vertex
		{
			glm::vec3 Position;
			glm::vec3 Color;
			glm::vec3 Normal;
			glm::vec2 UV;

			bool operator==(const Vertex& other) const
			{
				return Position == other.Position && Color == other.Color && 
					Normal == other.Normal && UV == other.UV;
			}
		};

		struct Builder
		{
			std::vector<Vertex> Vertices{};
			std::vector<uint32_t> Indices{};

			void LoadModel(const std::string& filepath);
		};

		Model(const Builder& builder);
		~Model();

		Model(const Model&) = delete;
		Model& operator=(const Model&) = delete;

		void Bind(VkCommandBuffer commandBuffer);
		void Draw(VkCommandBuffer commandBuffer);

		static Scope<Model> Create(const std::string& filepath);

	private:
		void CreateVertexBuffers(const std::vector<Vertex>& vertices);
		void CreateIndexBuffers(const std::vector<uint32_t>& indices);

	private:
		Scope<Buffer> m_VertexBuffer;
		uint32_t m_VertexCount = 0;

		Scope<Buffer> m_IndexBuffer;
		uint32_t m_IndexCount = 0;
	};
}