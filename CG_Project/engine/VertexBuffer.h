#pragma once
#include "defines.h"
#include "OpenGL.h"

namespace VE
{
	class VertexBuffer
	{
	public:
		VertexBuffer(const void* data, u32 size, u32 usage = GL_STATIC_DRAW);

		VertexBuffer(const VertexBuffer& other) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;

		VertexBuffer(VertexBuffer&& other);
		VertexBuffer& operator=(VertexBuffer&& other);

		~VertexBuffer();

		void Bind() const;
		void UnBind() const;

	private:
		u32 m_id;
	};
}