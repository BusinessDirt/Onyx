#include "stdafx.h"
#include "Model.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "Onyx/Util/Util.h"
#include "Onyx/Core/Application.h"

namespace std
{
	template<>
	struct hash<Onyx::Model::Vertex>
	{
		size_t operator()(Onyx::Model::Vertex const& vertex) const
		{
			size_t seed = 0;
			Onyx::HashCombine(seed, vertex.Position, vertex.Color, vertex.Normal, vertex.UV);
			return seed;
		}
	};
}

namespace Onyx
{
	Model::Model(const Builder& builder)
	{
		CreateVertexBuffers(builder.Vertices);
		CreateIndexBuffers(builder.Indices);
	}

	Model::~Model() 
	{
		m_IndexBuffer = nullptr;
		m_VertexBuffer = nullptr;
	}

	void Model::Bind(VkCommandBuffer commandBuffer)
	{
		VkBuffer buffers[] = { m_VertexBuffer->GetBuffer()};
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

		if (m_IndexBuffer)
		{
			vkCmdBindIndexBuffer(commandBuffer, m_IndexBuffer->GetBuffer(), 0, VK_INDEX_TYPE_UINT32);
		}
	}

	void Model::Draw(VkCommandBuffer commandBuffer)
	{
		if (m_IndexBuffer)
		{
			vkCmdDrawIndexed(commandBuffer, m_IndexCount, 1, 0, 0, 0);
			return;
		}

		vkCmdDraw(commandBuffer, m_VertexCount, 1, 0, 0);
	}

	Scope<Model> Model::Create(const std::string& filepath)
	{
		Builder builder{};
		builder.LoadModel(filepath);

		ONYX_CORE_INFO("Vertex count ({0}): {1}", filepath, builder.Vertices.size());

		return CreateScope<Model>(builder);
	}

	void Model::CreateVertexBuffers(const std::vector<Vertex>& vertices)
	{
		m_VertexCount = static_cast<uint32_t>(vertices.size());
		ONYX_CORE_ASSERT(m_VertexCount >= 3, "Vertex count must be at least 3");

		uint32_t vertexSize = sizeof(vertices[0]);
		VkDeviceSize bufferSize = vertexSize * m_VertexCount;

		Buffer stagingBuffer = Buffer(vertexSize, m_VertexCount, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		stagingBuffer.Map();
		stagingBuffer.WriteToBuffer((void*)vertices.data());

		m_VertexBuffer = CreateScope<Buffer>(vertexSize, m_VertexCount, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		Application::Get().GetDevice().CopyBuffer(stagingBuffer.GetBuffer(), m_VertexBuffer->GetBuffer(), bufferSize);
	}

	void Model::CreateIndexBuffers(const std::vector<uint32_t>& indices)
	{
		m_IndexCount = static_cast<uint32_t>(indices.size());
		if (m_IndexCount == 0) return;

		uint32_t indexSize = sizeof(indices[0]);
		VkDeviceSize bufferSize = indexSize * m_IndexCount;

		Buffer stagingBuffer = Buffer(indexSize, m_IndexCount, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		stagingBuffer.Map();
		stagingBuffer.WriteToBuffer((void*)indices.data());

		m_IndexBuffer = CreateScope<Buffer>(indexSize, m_IndexCount, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		Application::Get().GetDevice().CopyBuffer(stagingBuffer.GetBuffer(), m_IndexBuffer->GetBuffer(), bufferSize);
	}

	void Model::Builder::LoadModel(const std::string& filepath)
	{
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warn, err;

		bool result = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str());
		ONYX_CORE_ASSERT(result, warn + err);

		Vertices.clear();
		Indices.clear();

		std::unordered_map<Vertex, uint32_t> uniqueVertices{};

		for (const tinyobj::shape_t& shape : shapes)
		{
			for (const tinyobj::index_t& index : shape.mesh.indices)
			{
				Vertex vertex{};

				if (index.vertex_index >= 0)
				{
					vertex.Position = glm::vec3(
						attrib.vertices[3 * index.vertex_index + 0],
						attrib.vertices[3 * index.vertex_index + 1],
						attrib.vertices[3 * index.vertex_index + 2]
					);

					vertex.Color = glm::vec3(
						attrib.colors[3 * index.vertex_index + 0],
						attrib.colors[3 * index.vertex_index + 1],
						attrib.colors[3 * index.vertex_index + 2]
					);
				}

				if (index.normal_index >= 0)
				{
					vertex.Normal = glm::vec3(
						attrib.normals[3 * index.normal_index + 0],
						attrib.normals[3 * index.normal_index + 1],
						attrib.normals[3 * index.normal_index + 2]
					);
				}

				if (index.texcoord_index >= 0)
				{
					vertex.UV = glm::vec2(
						attrib.texcoords[2 * index.texcoord_index + 0],
						attrib.texcoords[2 * index.texcoord_index + 1]
					);
				}

				if (uniqueVertices.count(vertex) == 0)
				{
					uniqueVertices[vertex] = static_cast<uint32_t>(Vertices.size());
					Vertices.push_back(vertex);
				}

				Indices.push_back(uniqueVertices[vertex]);
			}
		}
	}
}


