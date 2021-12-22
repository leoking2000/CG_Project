#include "VertexBuffer.h"

namespace VE
{
	VertexBuffer::VertexBuffer(const void* data, u32 size, u32 usage)
	{
		glCall(glGenBuffers(1, &m_id));
		glCall(glBindBuffer(GL_ARRAY_BUFFER, m_id));
		glCall(glBufferData(GL_ARRAY_BUFFER, size, data, usage));
	}

	VertexBuffer::VertexBuffer(VertexBuffer&& other)
		:
		m_id(other.m_id)
	{
		other.m_id = 0;
	}

	VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other)
	{
		glCall(glDeleteBuffers(1, &m_id));

		m_id = other.m_id;
		other.m_id = 0;
		return *this;
	}

	VertexBuffer::~VertexBuffer()
	{
		glCall(glDeleteBuffers(1, &m_id));
	}

	void VertexBuffer::Bind() const
	{
		glCall(glBindBuffer(GL_ARRAY_BUFFER, m_id));
	}

	void VertexBuffer::UnBind() const
	{
		glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}
}


