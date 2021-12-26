#pragma once
#include "VE_Math.h"
#include <vector>
#include <string>

namespace VE
{
	class ObjLoader
	{
	public:
		struct Vertex
		{
			glm::vec3 pos;
			glm::vec2 texCord;
			glm::vec3 normal;
			glm::vec3 tangent;
			glm::vec3 bitangent;

			bool operator==(const Vertex& rhs)
			{
				return pos == rhs.pos &&
					texCord == rhs.texCord &&
					normal == rhs.normal &&
					tangent == rhs.tangent;
			}
		};
	public:
		class Mesh
		{
		public:
			std::vector<Vertex> vertices;
			std::vector<u32> indices;

			std::string BaseMap;
			std::string MaskMap;
			std::string NormalMap;
			//std::string Lightmap;
		};
	public:
		static std::vector<Mesh> Load(const char* filename);
	};
}