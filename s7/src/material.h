#pragma once

#include <bgfx/bgfx.h>
#include <bx/readerwriter.h>

namespace s7 {

	bgfx::ShaderHandle LoadShader(const char* _name);
	bgfx::ProgramHandle LoadProgram(const char* _vsName, const char* _fsName);
	bgfx::TextureHandle LoadTexture(const char* _name, uint32_t _flags = BGFX_TEXTURE_NONE, uint8_t _skip = 0, bgfx::TextureInfo* _info = NULL);
}
