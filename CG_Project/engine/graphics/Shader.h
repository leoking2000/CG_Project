#pragma once
#include "utilities/GL_Math.h"
#include "utilities/log.h"
#include <unordered_map>

namespace GL
{
	/*
	* Represents a Shader program
	*/
	class Shader
	{
	public:
		Shader(std::string filename);

		Shader(const Shader& other) = delete;
		Shader& operator=(const Shader& other) = delete;

		Shader(Shader&& other);
		Shader& operator=(Shader&& other);

		~Shader();

		void Bind() const;
		void UnBind() const;

		bool SetUniform(const std::string& name, float num) const;
		bool SetUniform(const std::string& name, glm::vec2 a) const;
		bool SetUniform(const std::string& name, glm::vec3 a) const;
		bool SetUniform(const std::string& name, glm::vec4 a) const;
		bool SetUniform(const std::string& name, int i) const;
		bool SetUniform(const std::string& name, const glm::mat4& mat) const;
	private:
		static u32 CreateShaderProgram(const char* filename);
		i32 GetLocation(const std::string& name) const;
	private:
		u32 m_id;
		mutable std::unordered_map<std::string, i32> m_uniforms;
	};

}