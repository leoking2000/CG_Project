#pragma once
#include "defines.h"
#include "VE_Math.h"
#include "OpenGL.h"
#include <vector>
#include <string>

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

		inline u32 GetWidth() const { return m_width; }
		inline u32 GetHeight() const { return m_height; }
		inline const std::string& GetName() const { return m_name; }

		void reload();

		void Bind(u32 slot = 0) const;
		void UnBind() const;

	private:
		u32 m_width;
		u32 m_height;
		u32 m_id;

		std::string m_name;
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
}