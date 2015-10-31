#include "mesh.h"

#include <bx/timer.h>
#include <bx/fpumath.h>

#include "entry/entry.h"

#include <tinystl/allocator.h>
#include <tinystl/vector.h>
#include <tinystl/string.h>
namespace stl = tinystl;

#include <bx/readerwriter.h>
#include <bx/string.h>
#include <ib-compress/indexbufferdecompression.h>

#include <bgfx/bgfx.h>


// Forward declaration
namespace bgfx {
	int32_t read(bx::ReaderI* _reader, bgfx::VertexDecl& _decl);
}

namespace s7 {

	Group::Group()
	{
		Reset();
	}

	void Group::Reset()
	{
		_vbh.idx = bgfx::invalidHandle;
		_ibh.idx = bgfx::invalidHandle;
		_prims.clear();
	}

	void Group::CalcBounds()
	{

	}

	Mesh::Mesh()
	{}

	Mesh::Mesh(bgfx::VertexDecl decl, int numGroups):
		_decl(decl),
		_groups(numGroups)
	{
	}
	
	Mesh* Mesh::Create(const char* filePath)
	{
		bx::FileReaderI* reader = entry::getFileReader();
		bx::open(reader, filePath);
		Mesh* mesh = Create(reader);
		bx::close(reader);
		return mesh;
	}

	Mesh* Mesh::Create(bx::ReaderSeekerI* reader)
	{
		Mesh* mesh = new Mesh();
		mesh->Load(reader);
		return mesh;
	}


	void Mesh::Load(bx::ReaderSeekerI* _reader)
	{
#define BGFX_CHUNK_MAGIC_VB  BX_MAKEFOURCC('V', 'B', ' ', 0x1)
#define BGFX_CHUNK_MAGIC_IB  BX_MAKEFOURCC('I', 'B', ' ', 0x0)
#define BGFX_CHUNK_MAGIC_IBC BX_MAKEFOURCC('I', 'B', 'C', 0x0)
#define BGFX_CHUNK_MAGIC_PRI BX_MAKEFOURCC('P', 'R', 'I', 0x0)

		using namespace bx;
		using namespace bgfx;

		Group group;

		bx::ReallocatorI* allocator = entry::getAllocator();

		uint32_t chunk;
		while (4 == bx::read(_reader, chunk))
		{
			switch (chunk)
			{
			case BGFX_CHUNK_MAGIC_VB:
			{
				read(_reader, &group._sphere, sizeof(Sphere));
				read(_reader, &group._aabb, sizeof(Aabb));
				read(_reader, &group._obb, sizeof(Obb));

				read(_reader, _decl);

				for (uint32_t attr = 0; attr < Attrib::Count; ++attr)
				{
					if (UINT16_MAX != _decl.m_attributes[attr])
					{
						uint8_t num;
						AttribType::Enum type;
						bool normalized;
						bool asInt;
						_decl.decode(Attrib::Enum(attr), num, type, normalized, asInt);

						dbgPrintf("\tattr %d - %d, num %d, type %d, norm %d, asint %d, offset %d\n"
							, attr
							, Attrib::Enum(attr)
							, num
							, type
							, normalized
							, asInt
							, _decl.m_offset[attr]
							);
					}
				}

				uint16_t stride = _decl.getStride();

				uint16_t numVertices;
				read(_reader, numVertices);
				const bgfx::Memory* mem = bgfx::alloc(numVertices*stride);
				read(_reader, mem->data, mem->size);

				group._vbh = bgfx::createVertexBuffer(mem, _decl);
			}
			break;

			case BGFX_CHUNK_MAGIC_IB:
			{
				uint32_t numIndices;
				read(_reader, numIndices);
				const bgfx::Memory* mem = bgfx::alloc(numIndices * 2);
				read(_reader, mem->data, mem->size);
				group._ibh = bgfx::createIndexBuffer(mem);
			}
			break;

			case BGFX_CHUNK_MAGIC_IBC:
			{
				uint32_t numIndices;
				bx::read(_reader, numIndices);

				const bgfx::Memory* mem = bgfx::alloc(numIndices * 2);

				uint32_t compressedSize;
				bx::read(_reader, compressedSize);

				void* compressedIndices = BX_ALLOC(allocator, compressedSize);

				bx::read(_reader, compressedIndices, compressedSize);

				ReadBitstream rbs((const uint8_t*)compressedIndices, compressedSize);
				DecompressIndexBuffer((uint16_t*)mem->data, numIndices / 3, rbs);

				BX_FREE(allocator, compressedIndices);

				group._ibh = bgfx::createIndexBuffer(mem);
			}
			break;

			case BGFX_CHUNK_MAGIC_PRI:
			{
				uint16_t len;
				read(_reader, len);

				stl::string material;
				material.resize(len);
				read(_reader, const_cast<char*>(material.c_str()), len);

				uint16_t num;
				read(_reader, num);

				for (uint32_t ii = 0; ii < num; ++ii)
				{
					read(_reader, len);

					stl::string name;
					name.resize(len);
					read(_reader, const_cast<char*>(name.c_str()), len);

					Primitive prim;
					read(_reader, prim.m_startIndex);
					read(_reader, prim.m_numIndices);
					read(_reader, prim.m_startVertex);
					read(_reader, prim.m_numVertices);
					read(_reader, &prim.m_sphere, sizeof(Sphere));
					read(_reader, &prim.m_aabb, sizeof(Aabb));
					read(_reader, &prim.m_obb, sizeof(Obb));

					group._prims.push_back(prim);
				}

				_groups.push_back(group);
				group.Reset();
			}
			break;

			default:
				DBG("%08x at %d", chunk, bx::skip(_reader, 0));
				break;
			}
		}
	}

	Group& Mesh::GetGroup(int i)
	{
		return _groups[i];
	}

	void Mesh::Unload()
	{
		for (auto it = _groups.begin(), itEnd = _groups.end(); it != itEnd; ++it)
		{
			const Group& group = *it;
			bgfx::destroyVertexBuffer(group._vbh);

			if (bgfx::isValid(group._ibh))
			{
				bgfx::destroyIndexBuffer(group._ibh);
			}
		}
		_groups.clear();
	}

	void Mesh::Submit(
		uint8_t id,
		bgfx::ProgramHandle program,
		const float* mtx,
		uint64_t state) const
	{
		if (BGFX_STATE_MASK == state)
		{
			state = 0
				| BGFX_STATE_RGB_WRITE
				| BGFX_STATE_ALPHA_WRITE
				| BGFX_STATE_DEPTH_WRITE
				| BGFX_STATE_DEPTH_TEST_LESS
				| BGFX_STATE_CULL_CCW
				| BGFX_STATE_MSAA
				;
		}

		uint32_t cached = bgfx::setTransform(mtx);

		for (auto it = _groups.begin(), itEnd = _groups.end(); it != itEnd; ++it)
		{
			const Group& group = *it;

			bgfx::setTransform(cached);
			bgfx::setIndexBuffer(group._ibh);
			bgfx::setVertexBuffer(group._vbh);
			bgfx::setState(state);
			bgfx::submit(id, program);
		}
	}

	void Mesh::Submit(
		const MeshState*const* state,
		uint8_t numPasses,
		const float* mtx,
		uint16_t numMatrices) const
	{
		uint32_t cached = bgfx::setTransform(mtx, numMatrices);

		for (uint32_t pass = 0; pass < numPasses; ++pass)
		{
			const MeshState& statePass = *state[pass];

			for (auto it = _groups.begin(), itEnd = _groups.end(); it != itEnd; ++it)
			{
				const Group& group = *it;

				bgfx::setTransform(cached, numMatrices);
				for (uint8_t tex = 0; tex < statePass.m_numTextures; ++tex)
				{
					const MeshState::Texture& texture = statePass.m_textures[tex];
					bgfx::setTexture(texture.m_stage
						, texture.m_sampler
						, texture.m_texture
						, texture.m_flags
						);
				}
				bgfx::setIndexBuffer(group._ibh);
				bgfx::setVertexBuffer(group._vbh);
				bgfx::setState(statePass.m_state);
				bgfx::submit(statePass.m_viewId, statePass.m_program);
			}
		}
	}

	MeshState* meshStateCreate()
	{
		MeshState* state = (MeshState*)BX_ALLOC(entry::getAllocator(), sizeof(MeshState));
		return state;
	}

	void meshStateDestroy(MeshState* _meshState)
	{
		BX_FREE(entry::getAllocator(), _meshState);
	}
}
