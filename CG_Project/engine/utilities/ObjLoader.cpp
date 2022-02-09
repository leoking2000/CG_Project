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
			LogError(msg);
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

	bool ObjLoader::Mesh::intersectRay_Local(const glm::vec3 origin, const glm::vec3 dir, f32& distance)
	{
		// store the current minimum distance found.
		distance = std::numeric_limits<f32>::max(); 

		bool found_isect = false;

		for (u32 i = 0; i < indices.size(); i += 3)
		{
			// read the triangle
			u32 indice_a = indices[i];
			u32 indice_b = indices[i + 1];
			u32 indice_c = indices[i + 2];
			glm::vec3 a = vertices[indice_a].pos;
			glm::vec3 b = vertices[indice_b].pos;
			glm::vec3 c = vertices[indice_c].pos;

			glm::vec3 barycoord;

			// check for intersection for that triangle
			if (glm::intersectRayTriangle(origin, dir, a, b, c, barycoord))
			{
				// find the position of the intersection
				const glm::vec3 temp_isect = a * barycoord.x + b * barycoord.y + c * barycoord.z;

				// find the distance between the origin on the intersection
				f32 temp_distance = glm::distance(origin, temp_isect);

				if (temp_distance < distance)
				{
					distance = temp_distance;
					found_isect = true;
				}
			}
		}

		return found_isect;
	}

}


