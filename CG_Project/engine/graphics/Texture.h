#pragma once
#include "utilities/defines.h"
#include "utilities/log.h"
#include <unordered_map>

namespace GL
{
	/*
	* Represents an image in GPU mem
	*/
	class Texture
	{
	public:
		Texture(const std::string& file_name);

		Texture(const Texture& other) = delete;
		Texture& operator=(const Texture& other) = delete;

		Texture(Texture&& other);
		Texture& operator=(Texture&& other);

		~Texture();

		void reload(const std::string& file_name);

		void Bind(u32 slot = 0) const;
		void UnBind() const;
	private:
		u32 m_id;
	};

	/*
	* Stores all the Textures, 
	*/
	class TextureManager
	{
	public:
		TextureManager(const TextureManager& other) = delete;
		TextureManager& operator=(const TextureManager& other) = delete;
	public:
		static Texture& GetTexture(const std::string& file_name);
		static void BindTexture(const std::string& file_name, u32 slot = 0);
		static void UnBindTexture(const std::string& file_name);
		static void Clear();
	private:
		TextureManager() {};
	private:
		static TextureManager& get();
	private:
		std::unordered_map<std::string, Texture> m_textures;
	};
}