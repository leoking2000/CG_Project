#pragma once
#include "defines.h"

namespace VE
{
	class FrameBuffer
	{
	public:
		FrameBuffer(u32 width, u32 height, bool is_depthmap = false);

		FrameBuffer(const FrameBuffer& other) = delete;
		FrameBuffer& operator=(const FrameBuffer& other) = delete;

		~FrameBuffer();
	public:
		void Resize(u32 width, u32 height) const;

		void Bind() const;
		void UnBind() const;

		void BindColorTexture(u32 slot) const;
		void BindDepthTexture(u32 slot) const;
	private:
		u32 m_color_texture;
		u32 m_depth_texture;
	
		u32 m_id;

		bool is_depthmap;
	};


}

u32 CheckFramebufferStatus(u32 framebuffer_object);