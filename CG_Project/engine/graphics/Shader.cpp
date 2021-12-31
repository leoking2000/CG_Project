#include "Shader.h"
#include "OpenGL.h"
#include "Log.h"

#include <malloc.h>
#include <fstream>
#include <string>
#include <sstream>

namespace GL
{
	Shader::Shader(std::string filename)
		:
		m_name(std::move(filename))
	{
		m_id = CreateShaderProgram(m_name.c_str());
		glCall(glUseProgram(m_id));
	}

	Shader::Shader(Shader&& other)
		:
		m_name(std::move(other.m_name)),
		m_id(other.m_id),
		m_uniforms(std::move(other.m_uniforms))
	{
		other.m_id = 0;
	}

	Shader& Shader::operator=(Shader&& other)
	{
		glCall(glDeleteProgram(m_id));

		m_name = std::move(other.m_name);
		m_id = other.m_id;
		m_uniforms = std::move(other.m_uniforms);

		other.m_id = 0;

		return *this;
	}

	Shader::~Shader()
	{
		glCall(glDeleteProgram(m_id));
	}

	void Shader::reload()
	{
		glCall(glDeleteProgram(m_id));

		m_id = CreateShaderProgram(m_name.c_str());
		glCall(glUseProgram(m_id));
	}

	void Shader::Bind() const
	{
		glCall(glUseProgram(m_id));
	}

	void Shader::UnBind() const
	{
		glCall(glUseProgram(0));
	}

	bool Shader::SetUniform(const std::string& name, float num) const
	{
		i32 location = GetLocation(name);
		if (location != -1)
		{
			glCall(glProgramUniform1f(m_id, location, num));
			return true;
		}
		std::string msg = "uniform error ";
		msg += name;
		LogError(msg);
		return false;
	}

	bool Shader::SetUniform(const std::string& name, glm::vec2 a) const
	{
		i32 location = GetLocation(name);
		if (location != -1)
		{
			glCall(glProgramUniform2f(m_id, location, a.x, a.y));
			return true;
		}
		std::string msg = "uniform error ";
		msg += name;
		LogError(msg);
		return false;
	}

	bool Shader::SetUniform(const std::string& name, glm::vec3 a) const
	{
		i32 location = GetLocation(name);
		if (location != -1)
		{
			glCall(glProgramUniform3f(m_id, location, a.x, a.y, a.z));
			return true;
		}
		std::string msg = "uniform error ";
		msg += name;
		LogError(msg);
		return false;
	}

	bool Shader::SetUniform(const std::string& name, glm::vec4 a) const
	{
		i32 location = GetLocation(name);
		if (location != -1)
		{
			glCall(glProgramUniform4f(m_id, location, a.x, a.y, a.z, a.w));
			return true;
		}
		std::string msg = "uniform error ";
		msg += name;
		LogError(msg);
		return false;
	}

	bool Shader::SetUniform(const std::string& name, int i) const
	{
		i32 location = GetLocation(name);
		if (location != -1)
		{
			glCall(glProgramUniform1i(m_id, location, i));
			return true;
		}
		std::string msg = "uniform error ";
		msg += name;
		LogError(msg);
		return false;
	}

	bool Shader::SetUniform(const std::string& name, const glm::mat4& mat) const
	{
		i32 location = GetLocation(name);
		if (location != -1)
		{
			glCall(glProgramUniformMatrix4fv(m_id, location, 1, GL_FALSE, glm::value_ptr(mat)));
			return true;
		}
		std::string msg = "uniform error ";
		msg += name;
		LogError(msg);
		return false;
	}



	i32 Shader::GetLocation(const std::string& name) const
	{
		if (m_uniforms.find(name) != m_uniforms.end())
		{
			return m_uniforms[name];
		}

		i32 loc;
		glCall(loc = glGetUniformLocation(m_id, name.c_str()));

		if (loc != -1)
		{
			m_uniforms[name] = loc;
		}

		return loc;
	}
}


u32 CompileShader(const char* source, u32 type)
{
	glCall(u32 id = glCreateShader(type));

	glCall(glShaderSource(id, 1, &source, nullptr));
	glCall(glCompileShader(id));

	int resoult;
	glCall(glGetShaderiv(id, GL_COMPILE_STATUS, &resoult));
	if (resoult == GL_FALSE)
	{
		LogError(type == GL_VERTEX_SHADER ? "<VERTEX SHADER COMPILE ERROR>" : "<FRAGMENT SHADER COMPILE ERROR>");
		int len;
		glCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len));
		char* msg = (char*)alloca(len * sizeof(char));
		glCall(glGetShaderInfoLog(id, len, &len, msg));
		LogError(msg);
		glCall(glDeleteShader(id));
		return 0;
	}

	return id;
}

u32 CreateShaderProgramVF(const char* vertexS, const char* fragS)
{
	glCall(u32 programid = glCreateProgram());

	u32 vs = CompileShader(vertexS, GL_VERTEX_SHADER);
	u32 fs = CompileShader(fragS, GL_FRAGMENT_SHADER);

	assert(vs != 0);
	assert(fs != 0);

	glCall(glAttachShader(programid, vs));
	glCall(glAttachShader(programid, fs));
	glCall(glLinkProgram(programid));
	glCall(glValidateProgram(programid));

	glCall(glDeleteShader(vs));
	glCall(glDeleteShader(fs));

	return programid;
}

u32 GL::Shader::CreateShaderProgram(const char* filename)
{
	assert(filename != nullptr);

	std::ifstream file(filename);

	assert(file.fail() == false);

	enum ShaderType
	{
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while (getline(file, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}

	u32 program = CreateShaderProgramVF(ss[0].str().c_str(), ss[1].str().c_str());

	std::string msg = "Creation of shader from file ";
	msg += filename;
	msg += " Has completed";
	LogInfo(msg);

	return program;
}

