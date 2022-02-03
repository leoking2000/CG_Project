#pragma once
#include "GL_Math.h"
#include "log.h"
#include <vector>

namespace GL
{
	/*
	* is a static class that loades a model from a file into CPU memory
	* a model is an array of meshs
	*/
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
					tangent == rhs.tangent &&
					bitangent == rhs.bitangent;
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
		public:
			/// <summary>
			/// returns true if a ray is intersecting with at least one triangle in the mesh.
			/// </summary>
			/// <param name="pos">The rays origine</param>
			/// <param name="dir">The rays direction</param>
			/// <param name="distance">Sets the variable to the closest intersection distance</param>
			//bool intersectRay_Local(const glm::vec3 pos, const glm::vec3 dir,
				//f32& distance);
		};
	public:
		static std::vector<Mesh> Load(const char* filename);
	private:
		ObjLoader();
	};
}