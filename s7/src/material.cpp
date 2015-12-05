#include "material.h"

#include "entry/entry.h"

#include <bx/string.h>


namespace s7 {

	static const bgfx::Memory* loadMem(
        bx::FileReaderI* reader,
        const char* filePath)
	{
		if (0 == bx::open(reader, filePath))
		{
			uint32_t size = (uint32_t)bx::getSize(reader);
			const bgfx::Memory* mem = bgfx::alloc(size + 1);
			bx::read(reader, mem->data, size);
			bx::close(reader);
			mem->data[mem->size - 1] = '\0';
			return mem;
		}

		return NULL;
	}

	static void* loadMem(
        bx::FileReaderI* reader,
        bx::AllocatorI* allocator,
        const char* filePath,
        uint32_t* actualSizePtr)
	{
		if (0 == bx::open(reader, filePath))
		{
			uint32_t size = (uint32_t)bx::getSize(reader);
			void* data = BX_ALLOC(allocator, size);
			bx::read(reader, data, size);
			bx::close(reader);

			if (NULL != actualSizePtr)
			{
				*actualSizePtr = size;
			}
			return data;
		}

		return NULL;
	}

	static bgfx::ShaderHandle loadShader(
        bx::FileReaderI* reader,
        const char* name)
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
		strcat(filePath, name);
		strcat(filePath, ".bin");

		return bgfx::createShader(loadMem(reader, filePath));
	}

	bgfx::ShaderHandle LoadShader(const char* name)
	{
		return loadShader(entry::getFileReader(), name);
	}

	bgfx::ProgramHandle loadProgram(
        bx::FileReaderI* reader,
        const char* vertName,
        const char* fragName)
	{
		bgfx::ShaderHandle vsh = loadShader(reader, vertName);
		bgfx::ShaderHandle fsh = BGFX_INVALID_HANDLE;
		if (fragName != nullptr)
		{
			fsh = loadShader(reader, fragName);
		}

		return bgfx::createProgram(vsh, fsh, true /* destroy shaders when program is destroyed */);
	}

	bgfx::ProgramHandle LoadProgram(const char* vertName, const char* fragName)
	{
		return loadProgram(entry::getFileReader(), vertName, fragName);
	}

	typedef unsigned char stbi_uc;
	extern "C" stbi_uc *stbi_load_from_memory(stbi_uc const *buffer, int len, int *x, int *y, int *comp, int req_comp);

	bgfx::TextureHandle loadTexture(
        bx::FileReaderI* reader,
        const char* name,
        uint32_t flags,
        uint8_t skip,
        bgfx::TextureInfo* info)
	{
		char filePath[512] = { '\0' };
		if (NULL == strchr(name, '/'))
		{
			strcpy(filePath, "textures/");
		}

		strcat(filePath, name);

		if (NULL != bx::stristr(name, ".dds")
			|| NULL != bx::stristr(name, ".pvr")
			|| NULL != bx::stristr(name, ".ktx"))
		{
			const bgfx::Memory* mem = loadMem(reader, filePath);
			if (NULL != mem)
			{
				return bgfx::createTexture(mem, flags, skip, info);
			}

			bgfx::TextureHandle handle = BGFX_INVALID_HANDLE;
			DBG("Failed to load %s.", filePath);
			return handle;
		}

		bgfx::TextureHandle handle = BGFX_INVALID_HANDLE;
		bx::AllocatorI* allocator = entry::getAllocator();

		uint32_t size = 0;
		void* data = loadMem(reader, allocator, filePath, &size);
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
				handle = bgfx::createTexture2D(
                    uint16_t(width), uint16_t(height),
                    1,
					bgfx::TextureFormat::RGBA8,
                    flags,
                    bgfx::copy(img, width*height * 4));

				free(img);

				if (info != nullptr)
				{
					bgfx::calcTextureSize(
                        *info,
                        uint16_t(width), uint16_t(height),
                        0,
                        false,
                        1,
                        bgfx::TextureFormat::RGBA8);
				}
			}
		}
		else
		{
			DBG("Failed to load %s.", filePath);
		}

		return handle;
	}

	bgfx::TextureHandle LoadTexture(
        const char* name,
        uint32_t flags,
        uint8_t skip,
        bgfx::TextureInfo* info)
	{
		return loadTexture(entry::getFileReader(), name, flags, skip, info);
	}
}
