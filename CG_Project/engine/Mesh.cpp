#include "Mesh.h"
#include "VE_Math.h"
#include <math.h>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "Log.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace VE
{
	ObjMesh::ObjMesh(std::vector<Vertex>& vertices, std::vector<u32>& indices, std::vector<std::string>& textures)
		:
		vertices(std::move(vertices)),
		indices(std::move(indices)),
		textures(std::move(textures))
	{
	}

	std::string GetFolderPath(const char* filename)
	{
		std::string str(filename);
		size_t found;
		found = str.find_last_of("/\\");
		return str.substr(0, found + 1);
	}

    std::vector<ObjMesh> loadObj(const char* filename)
    {
		LogInfo(std::string("Loading ") + filename);

		assert(filename != nullptr);

		Assimp::Importer imp;
		const aiScene* model = imp.ReadFile(filename, aiProcess_Triangulate | 
			aiProcess_JoinIdenticalVertices | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph);

		if (model == nullptr)
		{
			std::string msg(filename);
			msg += " Failed Loading";
			LogInfo(msg);
			return std::vector<ObjMesh>();
		}

		auto matirials = model->mMaterials;

		std::vector<ObjMesh> mesh_vector;

		for (u32 i = 0; i < model->mNumMeshes; i++)
		{
			const aiMesh* mesh = model->mMeshes[i];

			std::vector<ObjMesh::Vertex> vertices;
			std::vector<u32> indices;

			for (u32 v = 0; v < mesh->mNumVertices; v++)
			{
				ObjMesh::Vertex vert;

				vert.pos = glm::vec3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);

				if (mesh->mTextureCoords[0] != nullptr)
				{
					vert.texCord = glm::vec2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
				}
				else
				{
					vert.texCord = glm::vec2(0.0f, 0.0f);
				}

				vert.normal = glm::vec3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);

				vertices.push_back(vert);
			}

			for (uint32_t f = 0; f < mesh->mNumFaces; f++)
			{
				const aiFace& face = mesh->mFaces[f];
				assert(face.mNumIndices == 3);

				indices.push_back(face.mIndices[0]);
				indices.push_back(face.mIndices[1]);
				indices.push_back(face.mIndices[2]);
			}

			std::vector<std::string> textures;
			uint32_t matirialIndex = mesh->mMaterialIndex;

			if (matirialIndex >= 0)
			{
				auto& material = *matirials[matirialIndex];

				aiString texFileName;
				material.GetTexture(aiTextureType_DIFFUSE, 0, &texFileName);
				textures.emplace_back(GetFolderPath(filename) + texFileName.C_Str());

				material.GetTexture(aiTextureType_SPECULAR, 0, &texFileName);
				textures.emplace_back(GetFolderPath(filename) + texFileName.C_Str());
			}

			mesh_vector.emplace_back(vertices, indices, textures);
		}

		std::string msg(filename);
		msg += " Has been Loaded!!!";
		LogInfo(msg);
		return mesh_vector;
    }

	VE::Mesh::Mesh(VertexArray& va, VertexBuffer& vb, IndexBuffer& ib)
		:
		vertexArray(std::move(va)),
		vertexBuffer(std::move(vb)),
		indexBuffer(std::move(ib))
    {

    }

	Mesh::Mesh(VertexArray& va, VertexBuffer& vb, IndexBuffer& ib, std::vector<std::string>& textures)
		:
		vertexArray(std::move(va)),
		vertexBuffer(std::move(vb)),
		indexBuffer(std::move(ib))
	{
		for (auto& tex : textures)
		{
			this->textures.emplace_back(tex);
		}
	}

	Mesh::Mesh(VertexArray& va, VertexBuffer& vb, IndexBuffer& ib, Texture tex)
		:
		vertexArray(std::move(va)),
		vertexBuffer(std::move(vb)),
		indexBuffer(std::move(ib))
	{
		textures.push_back(std::move(tex));
	}

	Mesh Mesh::GenarateQuard(const char* filename, u32 repet)
	{
		float vertexs[] = {
			// pos                 // tex cord                      // normal
			 0.5f,  0.5f, 0.0f,    (float)repet, (float)repet,     0.0f,  0.0f, 1.0f, //0
			 0.5f, -0.5f, 0.0f,    (float)repet,         0.0f,     0.0f,  0.0f, 1.0f, //1
			-0.5f, -0.5f, 0.0f,			   0.0f,         0.0f,     0.0f,  0.0f, 1.0f, //2
			-0.5f,  0.5f, 0.0f,            0.0f, (float)repet,     0.0f,  0.0f, 1.0f, //3
		};

		VertexBuffer vertexBuffer(vertexs, sizeof(vertexs));

		ElementType arr[3] = { FLOAT3, FLOAT2, FLOAT3_N };
		Layout<3> layout(arr);

		VertexArray vertexArray;
		vertexArray.AddBuffer(vertexBuffer, layout);

		// index buffer
		uint32_t indices[] = {
			0, 1, 2,
			3, 0, 2
		};

		IndexBuffer indexBuffer(indices, 36);

		if (filename == nullptr)
		{
			return { vertexArray, vertexBuffer, indexBuffer };
		}

		return { vertexArray, vertexBuffer, indexBuffer, Texture(filename) };
	}

    Mesh Mesh::GenarateCube(const char* filename)
    {
		float vertexs[] = {
			// pos                 // tex cord      // normal
			// Forward
			-0.5f, -0.5f, -0.5f,    0.0f, 0.0f,     0.0f,  0.0f, -1.0f, //0
			-0.5f,  0.5f, -0.5f,    0.0f, 1.0f,     0.0f,  0.0f, -1.0f, //1
			 0.5f,  0.5f, -0.5f,    1.0f, 1.0f,     0.0f,  0.0f, -1.0f, //2
			 0.5f, -0.5f, -0.5f,    1.0f, 0.0f,     0.0f,  0.0f, -1.0f, //3

			// EAST							  
			 0.5f,  0.5f, -0.5f,     0.0f, 1.0f,    1.0f,  0.0f,  0.0f, //4
			 0.5f, -0.5f, -0.5f,     0.0f, 0.0f,    1.0f,  0.0f,  0.0f, //5
			 0.5f,  0.5f,  0.5f,     1.0f, 1.0f,    1.0f,  0.0f,  0.0f, //6
			 0.5f, -0.5f,  0.5f,     1.0f, 0.0f,    1.0f,  0.0f,  0.0f, //7

			// BACK							  
			 0.5f,  0.5f,  0.5f,     0.0f, 1.0f,    0.0f,  0.0f,  1.0f, //8
			 0.5f, -0.5f,  0.5f,     0.0f, 0.0f,    0.0f,  0.0f,  1.0f, //9
			-0.5f,  0.5f,  0.5f,     1.0f, 1.0f,    0.0f,  0.0f,  1.0f, //10
			-0.5f, -0.5f,  0.5f,     1.0f, 0.0f,    0.0f,  0.0f,  1.0f, //11

			// WEST							  
			-0.5f, -0.5f, -0.5f,     1.0f, 0.0f,   -1.0f,  0.0f,  0.0f, //12
			-0.5f,  0.5f, -0.5f,     1.0f, 1.0f,   -1.0f,  0.0f,  0.0f, //13
			-0.5f,  0.5f,  0.5f,     0.0f, 1.0f,   -1.0f,  0.0f,  0.0f, //14
			-0.5f, -0.5f,  0.5f,     0.0f, 0.0f,   -1.0f,  0.0f,  0.0f, //15

			// NORTH						  
			-0.5f,  0.5f, -0.5f,     0.0f, 0.0f,    0.0f,  1.0f,  0.0f,  //16
			 0.5f,  0.5f, -0.5f,     0.0f, 1.0f,    0.0f,  1.0f,  0.0f,  //17
			 0.5f,  0.5f,  0.5f,     1.0f, 1.0f,    0.0f,  1.0f,  0.0f,  //18
			-0.5f,  0.5f,  0.5f,     1.0f, 0.0f,    0.0f,  1.0f,  0.0f,  //19

			// SOUTH						  
			-0.5f, -0.5f, -0.5f,     0.0f, 0.0f,    0.0f, -1.0f,  0.0f,   //20
			 0.5f, -0.5f, -0.5f,     1.0f, 0.0f,    0.0f, -1.0f,  0.0f,   //21
			 0.5f, -0.5f,  0.5f,     1.0f, 1.0f,    0.0f, -1.0f,  0.0f,   //22
			-0.5f, -0.5f,  0.5f,     0.0f, 1.0f,    0.0f, -1.0f,  0.0f    //23
		};

		VertexBuffer vertexBuffer(vertexs, sizeof(vertexs));

		ElementType arr[3] = { FLOAT3, FLOAT2, FLOAT3_N };
		Layout<3> layout(arr);

		VertexArray vertexArray;
		vertexArray.AddBuffer(vertexBuffer, layout);

		// index buffer
		uint32_t indices[] = {
			// Forward
			0, 1, 2, /**/ 0, 2, 3,
			// EAST
			5, 4, 6, /**/ 5, 6, 7,
			// BACK
			9, 8, 10, /**/ 9, 10, 11,
			// WEST
			15, 14, 13, /**/ 15, 13, 12,
			// NORTH
			16, 19, 18, /**/ 16, 18, 17,
			// SOUTH
			22, 23, 20, /**/ 22, 20, 21
		};
		IndexBuffer indexBuffer(indices, 36);

		if (filename == nullptr)
		{
			return { vertexArray, vertexBuffer, indexBuffer };
		}

		return { vertexArray, vertexBuffer, indexBuffer, Texture(filename) };
    }

    Mesh Mesh::GenarateSphere(u32 prec, const char* filename)
    {
		const uint32_t numVertices = (prec + 1) * (prec + 1);
		const uint32_t numIndices = prec * prec * 6;

		std::vector<ObjMesh::Vertex> vertices(numVertices);

		// calculate triangle vertices
		for (u32 i = 0; i <= prec; i++) {
			for (u32 j = 0; j <= prec; j++) {
				float y = (float)cos(toRadians(180.0f - i * 180.0f / prec));
				float x = -(float)cos(toRadians(j * 360.0f / prec)) * (float)abs(cos(asin(y)));
				float z = (float)sin(toRadians(j * 360.0f / (float)(prec))) * (float)abs(cos(asin(y)));
				vertices[i * (prec + 1) + j].pos = glm::vec3(x, y, z);
				vertices[i * (prec + 1) + j].texCord = glm::vec2(((float)j / prec), ((float)i / prec));
				vertices[i * (prec + 1) + j].normal = glm::vec3(x, y, z);

			}
		}

		std::vector<uint32_t> indices(numIndices);

		// calculate triangle indices
		for (u32 i = 0; i < prec; i++) {
			for (u32 j = 0; j < prec; j++) {
				indices[6 * (i * prec + j) + 0] = i * (prec + 1) + j;
				indices[6 * (i * prec + j) + 1] = i * (prec + 1) + j + 1;
				indices[6 * (i * prec + j) + 2] = (i + 1) * (prec + 1) + j;
				indices[6 * (i * prec + j) + 3] = i * (prec + 1) + j + 1;
				indices[6 * (i * prec + j) + 4] = (i + 1) * (prec + 1) + j + 1;
				indices[6 * (i * prec + j) + 5] = (i + 1) * (prec + 1) + j;
			}
		}

		std::vector<float> vertex_buffer;

		for (ObjMesh::Vertex& v : vertices)
		{
			vertex_buffer.emplace_back(v.pos.x);
			vertex_buffer.emplace_back(v.pos.y);
			vertex_buffer.emplace_back(v.pos.z);

			vertex_buffer.emplace_back(v.texCord.s);
			vertex_buffer.emplace_back(v.texCord.t);

			vertex_buffer.emplace_back(v.normal.x);
			vertex_buffer.emplace_back(v.normal.y);
			vertex_buffer.emplace_back(v.normal.z);
		}

		VertexBuffer vertexBuffer((const void*)vertex_buffer.data(), (u32)(vertex_buffer.size() * sizeof(float)));

		ElementType arr[3] = { FLOAT3, FLOAT2, FLOAT3_N };
		Layout<3> layout(arr);

		VertexArray vertexArray;
		vertexArray.AddBuffer(vertexBuffer, layout);


		IndexBuffer indexBuffer(indices.data(), (u32)indices.size());

		if (filename == nullptr)
		{
			return { vertexArray, vertexBuffer, indexBuffer };
		}

		return { vertexArray, vertexBuffer, indexBuffer, Texture(filename) };
    }

	Mesh Mesh::Create(ObjMesh obj)
	{
		std::vector<float> vertex_buffer;

		for (auto& v : obj.vertices)
		{
			vertex_buffer.emplace_back(v.pos.x);
			vertex_buffer.emplace_back(v.pos.y);
			vertex_buffer.emplace_back(v.pos.z);

			vertex_buffer.emplace_back(v.texCord.s);
			vertex_buffer.emplace_back(v.texCord.t);

			vertex_buffer.emplace_back(v.normal.x);
			vertex_buffer.emplace_back(v.normal.y);
			vertex_buffer.emplace_back(v.normal.z);
		}

		VertexBuffer vertexBuffer((const void*)vertex_buffer.data(), (u32)(vertex_buffer.size() * sizeof(float)));

		ElementType arr[3] = { FLOAT3, FLOAT2, FLOAT3_N };
		Layout<3> layout(arr);

		VertexArray vertexArray;
		vertexArray.AddBuffer(vertexBuffer, layout);

		IndexBuffer indexBuffer(obj.indices.data(), (u32)obj.indices.size());

		return { vertexArray, vertexBuffer, indexBuffer, obj.textures };
	}

	Mesh Mesh::Skybox()
	{
		float skyboxVertices[] = {
			// positions          
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f
		};

		VertexBuffer vertexBuffer(skyboxVertices, sizeof(skyboxVertices));

		ElementType arr[1] = { FLOAT3 };
		Layout<1> layout(arr);

		VertexArray vertexArray;
		vertexArray.AddBuffer(vertexBuffer, layout);

		// index buffer
		uint32_t indices[] = {
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
			21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35
		};

		IndexBuffer indexBuffer(indices, 36);

		return { vertexArray, vertexBuffer, indexBuffer };
	}

	void Mesh::Draw(const glm::mat4& view, const glm::mat4& proj, const Shader& shader, bool use_mat)
	{
		this->Bind();
		shader.Bind();

		shader.SetUniform("proj", proj);
		shader.SetUniform("view", view);

		if (use_mat)
		{
			shader.SetUniform("mat.ambient", 0.3f);
			shader.SetUniform("mat.texture_diffuse", 0);
			//shader.SetUniform("mat.texture_specular", 1);
			//shader.SetUniform("mat.shininess", 200.0f);
		}


		glCall(glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr));
	}


	Model::Model(const char* filename)
		:
		transform(1.0f)
	{
		std::vector<ObjMesh> obj = loadObj(filename);

		for (u32 i = 0; i < obj.size(); i++)
		{
			m_mesh.emplace_back(Mesh::Create(obj[i]));
		}
	}

	Model::Model(Mesh&& mesh)
	{
		m_mesh.push_back(std::move(mesh));
	}

	void Model::Draw(const glm::mat4& view, const glm::mat4& proj, const Shader& shader, bool use_mat)
	{
		shader.SetUniform("model", transform);

		for (u32 i = 0; i < m_mesh.size(); i++)
		{
			m_mesh[i].Draw(view, proj, shader, use_mat);
		}
	}
}


