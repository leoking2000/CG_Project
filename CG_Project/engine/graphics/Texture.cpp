#include "Texture.h"
#include "OpenGL.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace GL
{
	Texture::Texture(const std::string& file_name)
	{
		m_id = 0;
		reload(file_name);
	}

	Texture::Texture(Texture&& other)
		:
		m_id(other.m_id)
	{
		other.m_id = 0;
	}

	Texture& Texture::operator=(Texture&& other)
	{
		glCall(glDeleteTextures(1, &m_id));

		m_id = other.m_id;

		other.m_id = 0;

		return *this;
	}

	Texture::~Texture()
	{
		glCall(glDeleteTextures(1, &m_id));
	}

	void Texture::reload(const std::string& file_name)
	{
		stbi_set_flip_vertically_on_load(1);

		int width;
		int height;
		int bpp;

		stbi_uc* data = stbi_load(file_name.c_str(), &width, &height, &bpp, 4);
		LogInfo(file_name + " is Loading....");
		assert(data != nullptr);

		glCall(glDeleteTextures(1, &m_id));

		glCall(glGenTextures(1, &m_id));
		glCall(glBindTexture(GL_TEXTURE_2D, m_id));

		glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

		glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		glCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));

		//glCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		//glCall(glGenerateMipmap(GL_TEXTURE_2D));

		glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));

		glCall(glBindTexture(GL_TEXTURE_2D, 0));
		stbi_image_free(data);

		LogInfo(file_name + " is Loaded!!!");
	}

	void Texture::Bind(u32 slot) const
	{
		glCall(glActiveTexture(GL_TEXTURE0 + slot));
		glCall(glBindTexture(GL_TEXTURE_2D, m_id));
	}

	void Texture::UnBind() const
	{
		glCall(glBindTexture(GL_TEXTURE_2D, 0));
	}

	///////////////////////////////////////////////////////////////

	Texture& TextureManager::GetTexture(const std::string& file_name)
	{
		TextureManager& manager = TextureManager::get();

		if (manager.m_textures.find(file_name) != manager.m_textures.end())
		{
			return manager.m_textures.at(file_name);
		}

		manager.m_textures.insert({ file_name, Texture(file_name) });

		return manager.m_textures.at(file_name);
	}

	void TextureManager::BindTexture(const std::string& file_name, u32 slot)
	{
		TextureManager::GetTexture(file_name).Bind(slot);
	}

	void TextureManager::UnBindTexture(const std::string& file_name)
	{
		TextureManager::GetTexture(file_name).UnBind();
	}

	void TextureManager::Clear()
	{
		TextureManager::get().m_textures.clear();
	}

	TextureManager& TextureManager::get()
	{
		static TextureManager manager;

		return manager;
	}

}


