#pragma once

#include "core.h"

#include <tinystl/allocator.h>
#include <tinystl/vector.h>

#include <bgfx/bgfx.h>
#include <bx/readerwriter.h>

namespace stl = tinystl;

namespace s7 {

	struct MeshState
	{
		struct Texture
		{
			uint32_t            m_flags;
			bgfx::UniformHandle m_sampler;
			bgfx::TextureHandle m_texture;
			uint8_t             m_stage;
		};

		Texture             m_textures[4];
		uint64_t            m_state;
		bgfx::ProgramHandle m_program;
		uint8_t             m_numTextures;
		uint8_t             m_viewId;
	};

	struct Primitive
	{
		uint32_t m_startIndex;
		uint32_t m_numIndices;
		uint32_t m_startVertex;
		uint32_t m_numVertices;

		Sphere m_sphere;
		Aabb m_aabb;
	};

	class Group
	{
	public:
		Sphere _sphere;
		Aabb _aabb;

		stl::vector<Primitive> _prims;

		bgfx::VertexBufferHandle _vbh;
		bgfx::IndexBufferHandle _ibh;

		Group();
		
		void Reset();
		void CalcBounds();
	};

	class Mesh
	{
	public:
		Mesh();
		Mesh(bgfx::VertexDecl decl, int numGroups);

		Group& GetGroup(int i);
        
        void CalcBounds();

		void Unload();

		void Submit(
			uint8_t id,
			bgfx::ProgramHandle program,
			const float* mtx,
			uint64_t state = BGFX_STATE_MASK) const;

		void Submit(
			const MeshState*const* state,
			uint8_t numPasses,
			const float* mtx,
			uint16_t numMatrices) const;

	private:
		bgfx::VertexDecl _decl;
		stl::vector<Group> _groups;
	};

	MeshState* meshStateCreate();
	void meshStateDestroy(MeshState* _meshState);
}
