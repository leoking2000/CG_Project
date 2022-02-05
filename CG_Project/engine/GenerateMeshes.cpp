#include "GenerateMeshes.h"

namespace GL
{
	Mesh GL::Create(const ObjLoader::Mesh& obj)
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

			vertex_buffer.emplace_back(v.tangent.x);
			vertex_buffer.emplace_back(v.tangent.y);
			vertex_buffer.emplace_back(v.tangent.z);

			vertex_buffer.emplace_back(v.bitangent.x);
			vertex_buffer.emplace_back(v.bitangent.y);
			vertex_buffer.emplace_back(v.bitangent.z);
		}

		VertexBuffer vertexBuffer((const void*)vertex_buffer.data(), (u32)(vertex_buffer.size() * sizeof(float)));

		ElementType arr[5] = { FLOAT3, FLOAT2, FLOAT3_N, FLOAT3_N, FLOAT3_N };
		Layout<5> layout(arr);

		VertexArray vertexArray;
		vertexArray.AddBuffer(vertexBuffer, layout);

		IndexBuffer indexBuffer(obj.indices.data(), (u32)obj.indices.size());

		Mesh mesh{ vertexArray, indexBuffer,  glm::vec3(1.0f, 1.0f, 1.0f) };

		mesh.BaseMap = obj.BaseMap;
		mesh.MaskMap = obj.MaskMap;
		mesh.NormalMap = obj.NormalMap;

		return mesh;
	}


	Mesh GenarateCube()
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

		Mesh mesh{ vertexArray, indexBuffer,  glm::vec3(1.0f, 1.0f, 1.0f) };

		return mesh;
	}


	Mesh GenarateSphere(u32 prec)
	{
		const uint32_t numVertices = (prec + 1) * (prec + 1);
		const uint32_t numIndices = prec * prec * 6;

		std::vector<ObjLoader::Vertex> vertices(numVertices);

		// calculate triangle vertices
		for (u32 i = 0; i <= prec; i++) 
		{
			for (u32 j = 0; j <= prec; j++) 
			{
				float y =  (float)glm::cos(glm::radians(180.0f - i * 180.0f / prec));
				float x = -(float)glm::cos(glm::radians(j * 360.0f / prec)) * (float)abs(cos(asin(y)));
				float z =  (float)glm::sin(glm::radians(j * 360.0f / (float)(prec))) * (float)abs(cos(asin(y)));

				vertices[i * (prec + 1) + j].pos = glm::vec3(x, y, z);
				vertices[i * (prec + 1) + j].texCord = glm::vec2(((float)j / prec), ((float)i / prec));
				vertices[i * (prec + 1) + j].normal = glm::vec3(x, y, z);
			}
		}

		std::vector<uint32_t> indices(numIndices);

		// calculate triangle indices
		for (u32 i = 0; i < prec; i++) 
		{
			for (u32 j = 0; j < prec; j++) 
			{
				indices[6 * (i * prec + j) + 0] = i * (prec + 1) + j;
				indices[6 * (i * prec + j) + 1] = i * (prec + 1) + j + 1;
				indices[6 * (i * prec + j) + 2] = (i + 1) * (prec + 1) + j;
				indices[6 * (i * prec + j) + 3] = i * (prec + 1) + j + 1;
				indices[6 * (i * prec + j) + 4] = (i + 1) * (prec + 1) + j + 1;
				indices[6 * (i * prec + j) + 5] = (i + 1) * (prec + 1) + j;
			}
		}

		std::vector<float> vertex_buffer;

		for (ObjLoader::Vertex& v : vertices)
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

		Mesh mesh{ vertexArray, indexBuffer,  glm::vec3(1.0f, 1.0f, 1.0f) };

		return mesh;
	}


	Mesh GenarateQuard(u32 repet)
	{
		float vertexs[] = {
			// pos                 // tex cord                      // normal			// tan				// bi
			 0.5f,  0.5f, 0.0f,    (float)repet, (float)repet,     0.0f,  0.0f, 1.0f,  -1.0f,  0.0f, 0.0f,	0.0f,  -1.0f, 0.0f,//0
			 0.5f, -0.5f, 0.0f,    (float)repet,         0.0f,     0.0f,  0.0f, 1.0f,  -1.0f,  0.0f, 0.0f,	0.0f,  -1.0f, 0.0f,//1
			-0.5f, -0.5f, 0.0f,		   0.0f,         0.0f,     0.0f,  0.0f, 1.0f,  -1.0f,  0.0f, 0.0f,	0.0f,  -1.0f, 0.0f,//2
			-0.5f,  0.5f, 0.0f,           0.0f, (float)repet,     0.0f,  0.0f, 1.0f,  -1.0f,  0.0f, 0.0f,	0.0f,  -1.0f, 0.0f //3
		};

		VertexBuffer vertexBuffer(vertexs, sizeof(vertexs));

		ElementType arr[5] = { FLOAT3, FLOAT2, FLOAT3_N, FLOAT3_N, FLOAT3_N };
		Layout<5> layout(arr);

		VertexArray vertexArray;
		vertexArray.AddBuffer(vertexBuffer, layout);

		// index buffer
		uint32_t indices[] = {
			0, 1, 2,
			3, 0, 2
		};

		IndexBuffer indexBuffer(indices, 6);

		Mesh mesh{ vertexArray, indexBuffer,  glm::vec3(1.0f, 1.0f, 1.0f) };

		return mesh;
	}
}


