#include "ktx_loader.hpp"
#include "tinyktx.h"
#include <core/memory/Buffer.hpp>
#include <core/log.h>
#include <core/memory/Memory.hpp>

namespace ari::gfx
{
	//------------------------------------------------------------------------------
	static void tinyktxCallbackError(void* user, char const* msg) {
		log_error("Tiny_Ktx ERROR: %s", msg);
	}

	//------------------------------------------------------------------------------
	static void* tinyktxCallbackAlloc(void* user, size_t size) {
		return core::Memory::Alloc(int(size));
	}

	//------------------------------------------------------------------------------
	static void tinyktxCallbackFree(void* user, void* data) {
		core::Memory::Free(data);
	}

	//------------------------------------------------------------------------------
	static size_t tinyktxCallbackRead(void* user, void* data, size_t size) {
		auto buffer = reinterpret_cast<core::Buffer*>(user);
		return buffer->Read(data, int(size));
	}

	//------------------------------------------------------------------------------
	static bool tinyktxCallbackSeek(void* user, int64_t offset) {
		auto buffer = reinterpret_cast<core::Buffer*>(user);
		return buffer->Seek(int(offset));
	}

	//------------------------------------------------------------------------------
	static int64_t tinyktxCallbackTell(void* user) {
		const auto buffer = reinterpret_cast<core::Buffer*>(user);
		return buffer->Tell();
	}

	//------------------------------------------------------------------------------
	sg_pixel_format GetPixelFormatFromTinyktxFormat(TinyKtx_Format format)
	{
		switch (format)
		{
		case TKTX_BC1_RGBA_UNORM_BLOCK:
			return SG_PIXELFORMAT_BC1_RGBA;
		case TKTX_BC2_UNORM_BLOCK:
			return SG_PIXELFORMAT_BC2_RGBA;
		case TKTX_BC3_UNORM_BLOCK:
			return SG_PIXELFORMAT_BC3_RGBA;
		case TKTX_ETC2_R8G8B8_UNORM_BLOCK:
			return SG_PIXELFORMAT_ETC2_RGB8;
		case TKTX_ETC2_R8G8B8A1_UNORM_BLOCK:
			return SG_PIXELFORMAT_ETC2_RGB8A1;
		case TKTX_ETC2_R8G8B8A8_UNORM_BLOCK:
			return SG_PIXELFORMAT_ETC2_RGBA8;
		default:
			log_warn("Unsupported texture format %d", format);
			return SG_PIXELFORMAT_NONE;
		}
	}

    void LoadKtxTexture(sg_image img, core::Buffer* buffer)
    {
		TinyKtx_Callbacks callbacks{
						&tinyktxCallbackError,
						&tinyktxCallbackAlloc,
						&tinyktxCallbackFree,
						&tinyktxCallbackRead,
						&tinyktxCallbackSeek,
						&tinyktxCallbackTell
		};
		// Load with tinyktx
		auto ctx = TinyKtx_CreateContext(&callbacks, buffer);
		if (!TinyKtx_ReadHeader(ctx))
		{
			log_error("Can't load the texture. Not a valid KTX format.");
			TinyKtx_DestroyContext(ctx);
			sg_fail_image(img);
			return;
		}
		uint32_t w = TinyKtx_Width(ctx);
		uint32_t h = TinyKtx_Height(ctx);
		uint32_t d = TinyKtx_Depth(ctx);
		uint32_t s = TinyKtx_ArraySlices(ctx);
		sg_pixel_format fmt = GetPixelFormatFromTinyktxFormat(TinyKtx_GetFormat(ctx));
		if (fmt == SG_PIXELFORMAT_NONE)
		{
			log_error("Can't load the texture. Unknown format.");
			TinyKtx_DestroyContext(ctx);
			sg_fail_image(img);
			return;
		}


		sg_image_desc desc;
		core::Memory::Fill(&desc, sizeof(sg_image_desc), 0);
		desc.type = SG_IMAGETYPE_2D;
		desc.render_target = false;
		desc.width = w;
		desc.height = h;
		desc.num_slices = d;
		desc.num_mipmaps = int(TinyKtx_NumberOfMipmaps(ctx));
		desc.pixel_format = fmt;
		desc.min_filter = SG_FILTER_LINEAR_MIPMAP_LINEAR;
		desc.mag_filter = SG_FILTER_LINEAR;

		for (auto i = 0u; i < TinyKtx_NumberOfMipmaps(ctx); ++i) {
			desc.content.subimage[0][i].size = TinyKtx_ImageSize(ctx, i);
			void* pixels = core::Memory::Alloc(desc.content.subimage[0][i].size);
			desc.content.subimage[0][i].ptr = pixels;

			core::Memory::Copy(TinyKtx_ImageRawData(ctx, i), pixels, desc.content.subimage[0][i].size);
			if (w > 1) w = w / 2;
			if (h > 1) h = h / 2;
			if (d > 1) d = d / 2;
		}

		TinyKtx_DestroyContext(ctx);

		sg_init_image(img, &desc);
    }
} // namespace ari::gfx
