#pragma once
#include "defines.h"
#include "VE_Math.h"
#include "OpenGL.h"
#include <vector>
#include <string>
#include <unordered_map>

namespace VE
{
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

	class CubeMap
	{
	public:
		CubeMap();
		CubeMap(const std::vector<std::string>& filenames);

		CubeMap(const CubeMap& other) = delete;
		CubeMap& operator=(const CubeMap& other) = delete;

		void reload(const std::vector<std::string>& filenames);

		CubeMap(CubeMap&& other);
		CubeMap& operator=(CubeMap&& other);

		inline bool isInitialized() { return m_id != 0; }

		~CubeMap();
	public:
		void Bind(u32 slot = 0) const;
		void UnBind() const;
	private:
		u32 m_id;
	};

	class TextureManager
	{
	public:
		TextureManager(const TextureManager& other) = delete;
		TextureManager& operator=(const TextureManager& other) = delete;
	public:
		static Texture& GetTexture(const std::string& name);
		static void Clear();
	private:
		TextureManager() {};
	private:
		static TextureManager& get();
	private:
		std::unordered_map<std::string, Texture> m_textures;
	};
}