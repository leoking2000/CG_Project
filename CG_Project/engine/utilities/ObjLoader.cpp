#include "ObjLoader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace GL
{
	std::vector<ObjLoader::Mesh> ObjLoader::Load(const char* filename)
	{
		LogInfo(std::string("Loading ") + filename);

		assert(filename != nullptr);

		Assimp::Importer imp;
		const aiScene* model = imp.ReadFile(filename, aiProcess_Triangulate | aiProcess_CalcTangentSpace |
			aiProcess_JoinIdenticalVertices | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph);

		if (model == nullptr)
		{
			std::string msg(filename);
			msg += " Failed Loading";
			LogInfo(msg);
			return std::vector<ObjLoader::Mesh>();
		}

		auto matirials = model->mMaterials;

		std::vector<ObjLoader::Mesh> mesh_vector;

		for (u32 i = 0; i < model->mNumMeshes; i++)
		{
			const aiMesh* mesh = model->mMeshes[i];

			std::vector<ObjLoader::Vertex> in_vertices;
			std::vector<u32> in_indices;

			for (u32 v = 0; v < mesh->mNumVertices; v++)
			{
				ObjLoader::Vertex vert;

				vert.pos = glm::vec3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);

				if (mesh->mTextureCoords[0] != nullptr)
				{
					vert.texCord = glm::vec2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);

					vert.normal = glm::vec3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
					vert.tangent = glm::vec3(mesh->mTangents[v].x, mesh->mTangents[v].y, mesh->mTangents[v].z);
					vert.bitangent = glm::vec3(mesh->mBitangents[v].x, mesh->mBitangents[v].y, mesh->mBitangents[v].z);
				}
				else
				{
					vert.texCord = glm::vec2(0.0f);
					vert.normal = glm::vec3(0.0f);
					vert.tangent = glm::vec3(0.0f);
					vert.bitangent = glm::vec3(0.0f);
				}

				in_vertices.push_back(vert);
			}

			for (uint32_t f = 0; f < mesh->mNumFaces; f++)
			{
				const aiFace& face = mesh->mFaces[f];
				assert(face.mNumIndices == 3);

				in_indices.push_back(face.mIndices[0]);
				in_indices.push_back(face.mIndices[1]);
				in_indices.push_back(face.mIndices[2]);
			}

			std::string in_BaseMap;
			std::string in_MaskMap;
			std::string in_NormalMap;

			uint32_t matirialIndex = mesh->mMaterialIndex;

			if (matirialIndex >= 0)
			{
				auto& material = *matirials[matirialIndex];
				aiString texFileName;

				material.GetTexture(aiTextureType_DIFFUSE, 0, &texFileName);
				in_BaseMap = std::string("assets/") + texFileName.C_Str();

				material.GetTexture(aiTextureType_SPECULAR, 0, &texFileName);
				in_MaskMap = std::string("assets/") + texFileName.C_Str();

				material.GetTexture(aiTextureType_HEIGHT, 0, &texFileName);
				in_NormalMap = std::string("assets/") + texFileName.C_Str();
			}

			ObjLoader::Mesh in_mesh;

			in_mesh.vertices = std::move(in_vertices);
			in_mesh.indices = std::move(in_indices);
			in_mesh.BaseMap = std::move(in_BaseMap);
			in_mesh.MaskMap = std::move(in_MaskMap);
			in_mesh.NormalMap = std::move(in_NormalMap);


			mesh_vector.push_back(std::move(in_mesh));
		}


		std::string msg(filename);
		msg += " Has been Loaded!!! (sub_meshs: " + std::to_string(mesh_vector.size()) + ")";
		LogInfo(msg);
		return mesh_vector;
	}

	/*
	bool ObjLoader::Mesh::intersectRay_Local(const glm::vec3 pos, const glm::vec3 dir, f32& distance)
	{
		const glm::vec3 normDir = glm::normalize(dir);

		distance = std::numeric_limits<float>::max();
		f32 curMin = distance;

		bool iters_found = false;
		glm::vec3 iters_point;

		for (u32 index = 0; index < indices.size(); index += 3)
		{
			u32 i = indices[index];

			Vertex& a = vertices[i];
			Vertex& b = vertices[i + 1];
			Vertex& c = vertices[i + 2];

			glm::vec3 barycoord;

			if (glm::intersectRayTriangle(pos, normDir, a.pos, b.pos, c.pos, barycoord))
			{
				const glm::vec3 tmp_isect = a.pos * barycoord.x + b.pos * barycoord.y + c.pos * barycoord.z;
				float dist = glm::distance(pos, tmp_isect);

				if (dist < curMin)
				{
					curMin = dist;
					distance = dist;
					iters_found = true;
					iters_point = tmp_isect;
				}
			}
		}

		return iters_found;
	}
	*/

}


