#pragma once

namespace ari::gfx
{
	struct Viewport
	{
	public:

		sx_rect						Rect;
		//bgfx::TextureFormat::Enum	TextureFormat = bgfx::TextureFormat::Count;
		bool						CreateDepth = false;
		bool						UseMSAA = false;

		// internal

		//bgfx::FrameBufferHandle		m_frame_buffer_handle = BGFX_INVALID_HANDLE;
		sx_rect						m_last_rect;
		//bgfx::ViewId				m_view_id = 0;
		//Texture						m_texture,
		//							m_depth_texture;
	};

} // ari::gfx
