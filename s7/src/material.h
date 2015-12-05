#pragma once

#include <bgfx/bgfx.h>
#include <bx/readerwriter.h>

namespace s7 {

	bgfx::ShaderHandle LoadShader(
        const char* name);
    
	bgfx::ProgramHandle LoadProgram(
        const char* vertName,
        const char* fragName);
    
	bgfx::TextureHandle LoadTexture(
        const char* name,
        uint32_t flags = BGFX_TEXTURE_NONE,
        uint8_t skip = 0,
        bgfx::TextureInfo* info = nullptr);
}
