#pragma once
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VE_Math.h"
#include "Texture.h"
#include <string>
#include <vector>
#include <Shader.h>

namespace VE
{
	class ObjMesh
	{
	public:
		struct Vertex
		{
			glm::vec3 pos;
			glm::vec2 texCord;
			glm::vec3 normal;

			bool operator==(const Vertex& rhs)
			{
				return pos == rhs.pos &&
					texCord == rhs.texCord &&
					normal == rhs.normal;
			}
		};
	public:
		ObjMesh(std::vector<Vertex>& vertices, std::vector<u32>& indices, std::vector<std::string>& textures);
	public:
		std::vector<Vertex> vertices;
		std::vector<u32> indices;
		std::vector<std::string> textures;
	};

	std::vector<ObjMesh> loadObj(const char* filename);

	class Mesh
	{
	public:
		static Mesh GenarateCube(const char* filename = nullptr);
		static Mesh GenarateSphere(u32 prec = 48, const char* filename = nullptr);
		static Mesh GenarateQuard(const char* filename = nullptr, u32 repet = 1);
		static Mesh Create(ObjMesh obj);

		static Mesh Skybox();
	public:
		void Draw(const glm::mat4& view, const glm::mat4& proj, const Shader& shader, bool use_mat = true);

		void Bind() const
		{
			vertexArray.Bind();
			indexBuffer.Bind();

			for (u32 i = 0; i < textures.size(); i++)
			{
				textures[i].Bind(i);
			}
		}

		std::vector<Texture> textures;
		VertexArray vertexArray;
		VertexBuffer vertexBuffer;
		IndexBuffer indexBuffer;
	private:
		Mesh(VertexArray& va, VertexBuffer& vb, IndexBuffer& ib);
		Mesh(VertexArray& va, VertexBuffer& vb, IndexBuffer& ib, std::vector<std::string>& tex);
		Mesh(VertexArray& va, VertexBuffer& vb, IndexBuffer& ib, Texture tex);
	};

	class Model
	{
	public:
		Model(const char* filename);
		Model(Mesh&& mesh);

		void Draw(const glm::mat4& view, const glm::mat4& proj, const Shader& shader, bool use_mat = true);

	public:
		glm::mat4 transform;
		std::vector<Mesh> m_mesh;
	};

}