#pragma once
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VE_Math.h"
#include "Texture.h"
#include <string>
#include <vector>
#include "Shader.h"

#include "ObjLoader.h"

namespace GL
{
	class Mesh
	{
	public:
		static Mesh GenarateCube();
		static Mesh GenarateSphere(u32 prec = 48);
		static Mesh GenarateQuard(u32 repet = 1);
		static Mesh Create(const ObjLoader::Mesh& obj);

		static Mesh Skybox();
	public:
		void Draw(const glm::mat4& view, const glm::mat4& proj, const Shader& shader, bool use_mat = true);
	public:
		VertexArray vertexArray;
		VertexBuffer vertexBuffer;
		IndexBuffer indexBuffer;

		std::string BaseMap;
		std::string MaskMap;
		std::string NormalMap;
		std::string Lightmap;
	private:
		Mesh(VertexArray& va, VertexBuffer& vb, IndexBuffer& ib);
	};

	class Model
	{
	public:
		Model(const char* filename);
		Model(Mesh&& mesh);

		void Draw(const glm::mat4& view, const glm::mat4& proj, const Shader& shader, bool use_mat = true);

	public:
		std::vector<Mesh> m_mesh;
	};

	/*
	* Stores all the Models
	*/
	class ModelManager
	{
	public:
		ModelManager(const ModelManager& other) = delete;
		ModelManager& operator=(const ModelManager& other) = delete;
	public:
		static Model& GetModel(const std::string& file_name);
		static void Clear();
	private:
		ModelManager() {};
	private:
		static ModelManager& get();
	private:
		std::unordered_map<std::string, Model> m_models;
	};

}