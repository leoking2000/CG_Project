#include "Mesh.h"
#include "Texture.h"
#include "OpenGL.h"

#include <fstream>
#include <sstream>
#include <algorithm>
#include "utilities/log.h"

namespace GL
{
	Mesh::Mesh(VertexArray& va, IndexBuffer& ib, const glm::vec3 color)
		:
		vertexArray(std::move(va)),
		indexBuffer(std::move(ib)),
		defaultColor(color)
    {

    }

	void Mesh::Draw(const glm::mat4& view, const glm::mat4& proj, const Shader& shader, bool use_mat)
	{
		vertexArray.Bind();
		indexBuffer.Bind();

		shader.Bind();

		shader.SetUniform("proj", proj);
		shader.SetUniform("view", view);

		if (use_mat)
		{
			if (BaseMap.empty())
			{
				shader.SetUniform("Has_BaseMap", 0);
				shader.SetUniform("BaseColor", defaultColor);
			}
			else
			{
				shader.SetUniform("Has_BaseMap", 1);

				TextureManager::BindTexture(BaseMap, 0);
				shader.SetUniform("BaseMap", 0);
			}

			if (NormalMap.empty())
			{
				shader.SetUniform("Has_NormalMap", 0);
			}
			else
			{
				shader.SetUniform("Has_NormalMap", 1);

				TextureManager::BindTexture(NormalMap, 1);
				shader.SetUniform("NormalMap", 1);
			}

			if (MaskMap.empty())
			{
				shader.SetUniform("Has_MaskMap", 0);
			}
			else
			{
				shader.SetUniform("Has_MaskMap", 1);

				TextureManager::GetTexture(MaskMap).Bind(2);
				shader.SetUniform("MaskMap", 2);
			}

			if (Lightmap.empty())
			{
				shader.SetUniform("Has_Lightmap", 0);
			}
			else
			{
				shader.SetUniform("Has_Lightmap", 1);

				TextureManager::GetTexture(Lightmap).Bind(3);
				shader.SetUniform("Lightmap", 3);
			}
		}

		glCall(glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr));
	}

}


