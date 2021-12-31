#pragma once
#include "OpenGL.h"
#include "defines.h"

namespace GL
{
	class IndexBuffer
	{
	public:
		IndexBuffer(const u32* data, u32 count, u32 usage = GL_STATIC_DRAW);

		IndexBuffer(const IndexBuffer& other) = delete;
		IndexBuffer& operator=(const IndexBuffer& other) = delete;

		IndexBuffer(IndexBuffer&& other);
		IndexBuffer& operator=(IndexBuffer&& other);

		~IndexBuffer();

		void Bind() const;
		void UnBind() const;

		inline u32 GetCount() const { return m_count; }

	private:
		u32 m_id;
		u32 m_count;
	};
}