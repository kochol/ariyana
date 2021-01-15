#include "basis_loader.hpp"
#include "transcoder/basisu_transcoder.h"
#include "core/memory/Memory.hpp"
#include "core/memory/Buffer.hpp"
#include <core/log.h>
#include "gfx/gfx.hpp"

using namespace basist;

namespace ari::gfx
{
    etc1_global_selector_codebook g_sel_codebook;

    sg_image_type GetImageType(basis_texture_type tx_type)
    {
        switch (tx_type)
        {
        case cBASISTexType2D:
            return SG_IMAGETYPE_2D;
        case cBASISTexType2DArray:
        case cBASISTexTypeVideoFrames:
            return SG_IMAGETYPE_ARRAY;
        case cBASISTexTypeCubemapArray:
            return SG_IMAGETYPE_CUBE;
        case cBASISTexTypeVolume:
            return SG_IMAGETYPE_3D;
        default:
            return _SG_IMAGETYPE_DEFAULT;
        }
    }

    bool GetTextureFormat(const basisu_image_info& image_info,
        sg_pixel_format& fmt,
        transcoder_texture_format& tx_fmt)
    {
        /*if (!image_info.m_alpha_flag && sg_query_pixelformat(SG_PIXELFORMAT_BC1_RGBA).sample)
        {
            fmt = SG_PIXELFORMAT_BC1_RGBA;
            tx_fmt = transcoder_texture_format::cTFBC1_RGB;
        }
        else if (sg_query_pixelformat(SG_PIXELFORMAT_BC3_RGBA).sample)
        {
            fmt = SG_PIXELFORMAT_BC3_RGBA;
            tx_fmt = transcoder_texture_format::cTFBC3_RGBA;
        }
        else*/ if (!image_info.m_alpha_flag && sg_query_pixelformat(SG_PIXELFORMAT_ETC2_RGB8).sample)
        {
            fmt = SG_PIXELFORMAT_ETC2_RGB8;
            tx_fmt = transcoder_texture_format::cTFETC1_RGB;
        }
        else if (image_info.m_alpha_flag && sg_query_pixelformat(SG_PIXELFORMAT_ETC2_RGBA8).sample)
        {
            fmt = SG_PIXELFORMAT_ETC2_RGBA8;
            tx_fmt = transcoder_texture_format::cTFETC2_RGBA;
        }
        else
        {
            log_error("Can't load the texture. GPU not supporting");
            return false;
        }

        return true;
    }

    void LoadBasisTexture(sg_image img, core::Buffer* buffer)
    {     
        basisu_transcoder transcoder(&g_sel_codebook);

        if (!transcoder.validate_header(buffer->Data(), buffer->Size()))
        {
            log_error("Can't load the texture. Not a valid basis format.");
            sg_fail_image(img);
            return;
        }

        auto tx_type = transcoder.get_texture_type(buffer->Data(), buffer->Size());
        uint32_t total_images = transcoder.get_total_images(buffer->Data(), buffer->Size());
        basisu_image_info image_info;
        transcoder.get_image_info(buffer->Data(), buffer->Size(), image_info, 0);
        uint32_t num_mips = transcoder.get_total_image_levels(buffer->Data(), buffer->Size(), 0);

        sg_pixel_format fmt;
        transcoder_texture_format tx_fmt;   

        if (!GetTextureFormat(image_info, fmt, tx_fmt))
        {
            sg_fail_image(img);
            return;
        }

        sg_image_desc desc;
        core::Memory::Fill(&desc, sizeof(sg_image_desc), 0);
        desc.type = GetImageType(tx_type);
        desc.render_target = false;
        desc.width = image_info.m_width;
        desc.height = image_info.m_height;
        desc.num_slices = total_images;
        desc.num_mipmaps = num_mips;
        desc.pixel_format = fmt;
        desc.min_filter = SG_FILTER_LINEAR_MIPMAP_LINEAR;
        desc.mag_filter = SG_FILTER_LINEAR;

        // get the texture data
        transcoder.start_transcoding(buffer->Data(), buffer->Size());
        uint32_t bytes_per_block = basis_get_bytes_per_block_or_pixel(tx_fmt);

        for (uint32_t i = 0; i < total_images; i++)
        {            
            for (uint32_t m = 0; m < num_mips; m++)
            {
                basisu_image_level_info info;
                transcoder.get_image_level_info(buffer->Data(), buffer->Size(), info, i, m);
                uint32_t image_sz = GetSurfacePitch((PixelFormat)fmt, info.m_width, info.m_height, 1);//  info.m_total_blocks* bytes_per_block;
               
                desc.content.subimage[i][m].size = image_sz;
                void* pixels = core::Memory::Alloc(image_sz);
                desc.content.subimage[i][m].ptr = pixels;

                transcoder.transcode_image_level(buffer->Data(), buffer->Size(),
                    i, m,
                    pixels, info.m_total_blocks,
                    tx_fmt);
            }
        }

        transcoder.stop_transcoding();

        sg_init_image(img, &desc);
    }
} // namespace ari::gfx
