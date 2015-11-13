#include "genmeshconverter.h"

namespace s7 {

	struct PackedVert
	{
		Vec3f _pos;
		Vec4u8 _normal;
	};


    Mesh Convert(const GenMesh& m)
    {
        // Declare vertex structure
        bgfx::VertexDecl myDecl;
		myDecl.begin();
		myDecl.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float, false, false);
		myDecl.add(bgfx::Attrib::Normal, 4, bgfx::AttribType::Uint8, true, true);
		myDecl.end();

		Mesh mesh(myDecl, 1);
		auto& group = mesh.GetGroup(0);

		// Allocate & assign vertices (and normals)
		// TODO Work out normals.
		// TODO Calc accurate points
		// TODO Break out/refactor
		{
			auto numVerts = 5;
			auto vertMem = bgfx::alloc(numVerts * myDecl.getStride());
			auto verts = reinterpret_cast<PackedVert*>(vertMem->data);

			verts[0]._pos.Set(0, 0, 1);
			verts[0]._normal.Set(255, 0, 0, 255);
			verts[1]._pos.Set(-1, 0, 0);
			verts[1]._normal.Set(255, 0, 0, 255);
			verts[2]._pos.Set(1, 0, 0);
			verts[2]._normal.Set(255, 0, 0, 255);
			verts[3]._pos.Set(0, -1, 0);
			verts[3]._normal.Set(255, 0, 0, 255);

			group._vbh = bgfx::createVertexBuffer(vertMem, myDecl);
		}

		// Allocate & assign indices
		{
			auto numIndices = 12;
			auto idxMem = bgfx::alloc(numIndices * 2);
			auto indices = reinterpret_cast<uint16_t*>(idxMem->data);

			indices[0] = 0;  indices[1] = 3;  indices[2] = 1;
			indices[3] = 0;  indices[4] = 2;  indices[5] = 3;
			indices[6] = 0;  indices[7] = 1;  indices[8] = 2;
			indices[9] = 1;  indices[10] = 3;  indices[11] = 2;

			group._ibh = bgfx::createIndexBuffer(idxMem);
		}

		// Create a primitive
		{
			group._prims.resize(1);
			auto& prim = group._prims[0];
			prim.m_startVertex = 0;
			prim.m_numVertices = 4;
			prim.m_startIndex = 0;
			prim.m_numIndices = 12;
		}

		// Calc bounds for prim and group
		mesh.CalcBounds();
        
        return mesh;
    }

}