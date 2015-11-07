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
	{
    }

	Mesh::Mesh(bgfx::VertexDecl decl, int numGroups):
		_decl(decl),
		_groups(numGroups)
	{
	}
	
    void Mesh::CalcBounds()
    {
        for (auto& group: _groups)
            group.CalcBounds();
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
