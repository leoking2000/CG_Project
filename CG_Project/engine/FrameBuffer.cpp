#include "FrameBuffer.h"
#include "OpenGL.h"
#include "log.h"


namespace VE
{
	VE::FrameBuffer::FrameBuffer(u32 width, u32 height, bool is_depthmap)
		:
		is_depthmap(is_depthmap)
	{
		glCall(glGenFramebuffers(1, &m_id));
		glCall(glGenTextures(1, &m_depth_texture));

		m_color_texture = 0;
		if (!is_depthmap)
		{
			glCall(glGenTextures(1, &m_color_texture));
		}
		
		Resize(width, height);
	}

	FrameBuffer::~FrameBuffer()
	{
		glCall(glDeleteTextures(1, &m_color_texture));
		glCall(glDeleteTextures(1, &m_depth_texture));
		glCall(glDeleteFramebuffers(1, &m_id));
	}

	void FrameBuffer::Resize(u32 width, u32 height) const
	{
		// texture
		if (!is_depthmap)
		{
			glCall(glBindTexture(GL_TEXTURE_2D, m_color_texture));
			glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
			glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
			glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
			glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
			glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0));
		}

		glCall(glBindTexture(GL_TEXTURE_2D, m_depth_texture));
		glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0));

		glCall(glBindTexture(GL_TEXTURE_2D, 0));

		// framebuffer to link to everything together
		glCall(glBindFramebuffer(GL_FRAMEBUFFER, m_id));

		if (is_depthmap)
		{
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
		}
		else
		{
			glCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_color_texture, 0));
		}
		
		glCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth_texture, 0));

		GLenum status = CheckFramebufferStatus(m_id);
		assert(status == GL_FRAMEBUFFER_COMPLETE);
	}

	void FrameBuffer::Bind() const
	{
		glCall(glBindFramebuffer(GL_FRAMEBUFFER, m_id));

		if (!is_depthmap)
		{
			GLenum drawbuffers[1] = { GL_COLOR_ATTACHMENT0 };
			glCall(glDrawBuffers(1, drawbuffers));
		}
	}

	void FrameBuffer::UnBind() const
	{
		glCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	void FrameBuffer::BindColorTexture(u32 slot) const
	{
		glCall(glActiveTexture(GL_TEXTURE0 + slot));
		glCall(glBindTexture(GL_TEXTURE_2D, m_color_texture));
	}

	void FrameBuffer::BindDepthTexture(u32 slot) const
	{
		glCall(glActiveTexture(GL_TEXTURE0 + slot));
		glCall(glBindTexture(GL_TEXTURE_2D, m_depth_texture));
	}

}

u32 CheckFramebufferStatus(u32 framebuffer_object)
{
	glCall(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_object));
	u32 status;
	if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
	{
		LogError(std::string("glCheckFramebufferStatus: error ") + std::to_string(status));
		switch (status)
		{
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			printf("Incomplete attatchement\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			printf("Incomplete missing attachment\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			printf("Incomplete draw buffer\n");
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED:
			printf("Unsupported\n");
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
			printf("Incomplete layer targets\n");
			break;
		default:
			printf("Default error\n");
			break;
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return status;
}