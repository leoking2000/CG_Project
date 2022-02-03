#include "Model.h"

namespace GL
{
	Model::Model(const char* filename)
	{
		std::vector<ObjLoader::Mesh> obj = ObjLoader::Load(filename);

		for (u32 i = 0; i < obj.size(); i++)
		{
			meshs.emplace_back(Create(obj[i]));
		}
	}

	Model::Model(Mesh&& mesh)
	{
		meshs.push_back(std::move(mesh));
	}

	void Model::Draw(const glm::mat4& view, const glm::mat4& proj, const Shader& shader, bool use_mat)
	{
		for (u32 i = 0; i < meshs.size(); i++)
		{
			meshs[i].Draw(view, proj, shader, use_mat);
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	Model& ModelManager::GetModel(const std::string& file_name)
	{
		ModelManager& manager = ModelManager::get();

		if (manager.m_models.find(file_name) != manager.m_models.end())
		{
			return manager.m_models.at(file_name);
		}

		manager.m_models.insert({ file_name, Model(file_name.c_str()) });

		return manager.m_models.at(file_name);
	}

	Model& ModelManager::GetCube()
	{
		ModelManager& manager = ModelManager::get();

		if (manager.m_models.find("cube") != manager.m_models.end())
		{
			return manager.m_models.at("cube");
		}

		return ModelManager::Make("cube", GenarateCube());
	}

	Model& ModelManager::GetSphere()
	{
		ModelManager& manager = ModelManager::get();

		if (manager.m_models.find("sphere") != manager.m_models.end())
		{
			return manager.m_models.at("sphere");
		}

		return ModelManager::Make("sphere", GenarateSphere());
	}

	Model& ModelManager::GetQuard()
	{
		ModelManager& manager = ModelManager::get();

		if (manager.m_models.find("quard") != manager.m_models.end())
		{
			return manager.m_models.at("quard");
		}

		return ModelManager::Make("quard", GenarateQuard());
	}

	Model& ModelManager::Make(const std::string& name, Mesh&& mesh)
	{
		ModelManager& manager = ModelManager::get();

		manager.m_models.insert({ name, Model(std::move(mesh)) });

		return manager.m_models.at(name);
	}

	void ModelManager::Clear()
	{
		ModelManager::get().m_models.clear();
	}

	ModelManager& ModelManager::get()
	{
		static ModelManager manager;

		return manager;
	}
}