#pragma once
#include "GenerateMeshes.h"

namespace GL
{
	class Model
	{
	public:
		Model(const char* filename);
		Model(Mesh&& mesh);

		void Draw(const glm::mat4& view, const glm::mat4& proj, const Shader& shader, bool use_mat = true);

	public:
		std::vector<Mesh> meshs;
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
		static Model& GetCube();
		static Model& GetSphere();
		static Model& GetQuard();
		static void Clear();
	private:
		ModelManager() {};
	private:
		static Model& Make(const std::string& name, Mesh&& mesh);
		static ModelManager& get();
	private:
		std::unordered_map<std::string, Model> m_models;
	};
}

