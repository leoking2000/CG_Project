#pragma once
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

namespace GL
{
	class Mesh
	{
	public:
		Mesh(VertexArray& va, IndexBuffer& ib, const glm::vec3 color);
	public:
		void Draw(const glm::mat4& view, const glm::mat4& proj, const Shader& shader, bool use_mat = true);
	public:
		glm::vec3 defaultColor;

		std::string BaseMap;
		std::string MaskMap;
		std::string NormalMap;
		std::string Lightmap;
	private:
		VertexArray vertexArray;
		IndexBuffer indexBuffer;
	};
}