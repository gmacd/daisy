#include "material.h"

#include "entry/entry.h"

#include <bx/string.h>


namespace s7 {

	static const bgfx::Memory* loadMem(bx::FileReaderI* _reader, const char* _filePath)
	{
		if (0 == bx::open(_reader, _filePath))
		{
			uint32_t size = (uint32_t)bx::getSize(_reader);
			const bgfx::Memory* mem = bgfx::alloc(size + 1);
			bx::read(_reader, mem->data, size);
			bx::close(_reader);
			mem->data[mem->size - 1] = '\0';
			return mem;
		}

		return NULL;
	}

	static void* loadMem(bx::FileReaderI* _reader, bx::AllocatorI* _allocator, const char* _filePath, uint32_t* _size)
	{
		if (0 == bx::open(_reader, _filePath))
		{
			uint32_t size = (uint32_t)bx::getSize(_reader);
			void* data = BX_ALLOC(_allocator, size);
			bx::read(_reader, data, size);
			bx::close(_reader);

			if (NULL != _size)
			{
				*_size = size;
			}
			return data;
		}

		return NULL;
	}

	static bgfx::ShaderHandle loadShader(bx::FileReaderI* _reader, const char* _name)
	{
		char filePath[512];

		const char* shaderPath = "shaders/dx9/";

		switch (bgfx::getRendererType())
		{
		case bgfx::RendererType::Direct3D11:
		case bgfx::RendererType::Direct3D12:
			shaderPath = "shaders/dx11/";
			break;

		case bgfx::RendererType::OpenGL:
			shaderPath = "shaders/glsl/";
			break;

		case bgfx::RendererType::Metal:
			shaderPath = "shaders/metal/";
			break;

		case bgfx::RendererType::OpenGLES:
			shaderPath = "shaders/gles/";
			break;

		default:
			break;
		}

		strcpy(filePath, shaderPath);
		strcat(filePath, _name);
		strcat(filePath, ".bin");

		return bgfx::createShader(loadMem(_reader, filePath));
	}

	bgfx::ShaderHandle LoadShader(const char* _name)
	{
		return loadShader(entry::getFileReader(), _name);
	}

	bgfx::ProgramHandle loadProgram(bx::FileReaderI* _reader, const char* _vsName, const char* _fsName)
	{
		bgfx::ShaderHandle vsh = loadShader(_reader, _vsName);
		bgfx::ShaderHandle fsh = BGFX_INVALID_HANDLE;
		if (NULL != _fsName)
		{
			fsh = loadShader(_reader, _fsName);
		}

		return bgfx::createProgram(vsh, fsh, true /* destroy shaders when program is destroyed */);
	}

	bgfx::ProgramHandle LoadProgram(const char* _vsName, const char* _fsName)
	{
		return loadProgram(entry::getFileReader(), _vsName, _fsName);
	}

	typedef unsigned char stbi_uc;
	extern "C" stbi_uc *stbi_load_from_memory(stbi_uc const *buffer, int len, int *x, int *y, int *comp, int req_comp);

	bgfx::TextureHandle loadTexture(bx::FileReaderI* _reader, const char* _name, uint32_t _flags, uint8_t _skip, bgfx::TextureInfo* _info)
	{
		char filePath[512] = { '\0' };
		if (NULL == strchr(_name, '/'))
		{
			strcpy(filePath, "textures/");
		}

		strcat(filePath, _name);

		if (NULL != bx::stristr(_name, ".dds")
			|| NULL != bx::stristr(_name, ".pvr")
			|| NULL != bx::stristr(_name, ".ktx"))
		{
			const bgfx::Memory* mem = loadMem(_reader, filePath);
			if (NULL != mem)
			{
				return bgfx::createTexture(mem, _flags, _skip, _info);
			}

			bgfx::TextureHandle handle = BGFX_INVALID_HANDLE;
			DBG("Failed to load %s.", filePath);
			return handle;
		}

		bgfx::TextureHandle handle = BGFX_INVALID_HANDLE;
		bx::AllocatorI* allocator = entry::getAllocator();

		uint32_t size = 0;
		void* data = loadMem(_reader, allocator, filePath, &size);
		if (NULL != data)
		{
			int width = 0;
			int height = 0;
			int comp = 0;

			uint8_t* img = NULL;
			img = stbi_load_from_memory((uint8_t*)data, size, &width, &height, &comp, 4);

			BX_FREE(allocator, data);

			if (NULL != img)
			{
				handle = bgfx::createTexture2D(uint16_t(width), uint16_t(height), 1
					, bgfx::TextureFormat::RGBA8
					, _flags
					, bgfx::copy(img, width*height * 4)
					);

				free(img);

				if (NULL != _info)
				{
					bgfx::calcTextureSize(*_info
						, uint16_t(width)
						, uint16_t(height)
						, 0
						, false
						, 1
						, bgfx::TextureFormat::RGBA8
						);
				}
			}
		}
		else
		{
			DBG("Failed to load %s.", filePath);
		}

		return handle;
	}

	bgfx::TextureHandle LoadTexture(const char* _name, uint32_t _flags, uint8_t _skip, bgfx::TextureInfo* _info)
	{
		return loadTexture(entry::getFileReader(), _name, _flags, _skip, _info);
	}
}
