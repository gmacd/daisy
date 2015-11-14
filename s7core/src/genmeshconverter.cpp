#include "genmeshconverter.h"

#include <algorithm>

namespace s7 {

	struct PackedVert
	{
		Vec3f pos;
		Vec4u8 normal;
	};


    Mesh Convert(const GenMesh& m)
    {
        // Declare vertex structure
        bgfx::VertexDecl decl;
		decl.begin();
		decl.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float, false, false);
		decl.add(bgfx::Attrib::Normal, 4, bgfx::AttribType::Uint8, true, true);
		decl.end();

		Mesh mesh(decl, 1);
		auto& group = mesh.GetGroup(0);


		// Allocate & assign vertices (and normals)
        auto vertMem = bgfx::alloc(m._verts.size() * decl.getStride());
        auto verts = reinterpret_cast<PackedVert*>(vertMem->data);
        for (auto i = 0; i < m._verts.size(); i++)
        {
            verts[i].pos = m._vertData[i].v;
            verts[i].normal = Vec4u8(255, 0, 0, 255);
        }
        group._vbh = bgfx::createVertexBuffer(vertMem, decl);


		// Allocate & assign indices
        // How many faces and triangles?
        // Work out how many indices needed.
        // 3 edges -> 1 triangle  -> 3 verts
        // 4 edges -> 2 triangles -> 6 verts
        // 5 edges -> 3 triangles -> 9 verts
        auto numIndices = 0;
        for (auto& f: m._faces)
        {
            auto numTrianglesInFace = m.GetNumEdgesInFace(&f)-2;
            numIndices += numTrianglesInFace*3;
        }
        auto idxMem = bgfx::alloc(numIndices * 2);
        auto indices = reinterpret_cast<uint16_t*>(idxMem->data);
        
        // Now triangulate the face, assuming that it's convex.
        auto i = 0;
        for (auto& f: m._faces)
        {
            // Get the first index
            GenEdge* startEdge = f._edge;
            auto firstIdx = startEdge->_vert->_vertIdx;
            
            // Second index
            GenEdge* currEdge = startEdge->_next;
            auto secondIdx = currEdge->_vert->_vertIdx;
            currEdge = currEdge->_next;

            // One triangle per remaining index
            while (currEdge != startEdge)
            {
                auto thirdIdx = currEdge->_vert->_vertIdx;
            
                indices[i++] = firstIdx;
                indices[i++] = secondIdx;
                indices[i++] = thirdIdx;
                
                secondIdx = thirdIdx;
                currEdge = currEdge->_next;
            }
        }

        group._ibh = bgfx::createIndexBuffer(idxMem);


		// Create a primitive
        group._prims.resize(1);
        auto& prim = group._prims[0];
        prim.m_startVertex = 0;
        prim.m_numVertices = m._verts.size();
        prim.m_startIndex = 0;
        prim.m_numIndices = 12;

		// Calc bounds for prim and group
		mesh.CalcBounds();
        
        return mesh;
    }

}