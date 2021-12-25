#pragma once
#include "Mesh.h"
#include "FrameBuffer.h"

namespace VE
{
	class DirectionalLight
	{
	public:
		DirectionalLight(const glm::vec3 dir = { 0.0f, 0.0f, 0.0f });

		void RenderShadowMap(const std::vector<Model>& models);

	public:
		glm::vec3 direction;

		// Shadows
		VE::Shader shadow_shader{ "Shader/shadow_map.glsl" };
		static constexpr u32 SHADOW_SIZE = 1024;
		FrameBuffer shadow_map{ SHADOW_SIZE, SHADOW_SIZE, true };

		glm::mat4 proj;
		glm::mat4 view;
	};
}