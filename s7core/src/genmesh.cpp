#include "genmesh.h"

#include <map>
#include <utility>

namespace s7 {

    GenMesh::GenMesh(uint32_t numVerts, uint32_t numEdges, uint32_t numFaces):
        _verts(numVerts),
        _edges(numEdges),
        _faces(numFaces)
    {
    }

    GenMesh::GenMesh(
        std::vector<Vec3f> verts,
        std::vector<std::vector<uint32_t>> faces)
    {
        auto numVerts = (uint32_t)verts.size();
        auto numFaces = (uint32_t)faces.size();
        
        auto totalNumEdges = 0;
        for (auto f: faces)
            totalNumEdges += f.size();
        
        _vertData.resize(numVerts);
        _verts.resize(numVerts);
        _edges.resize(totalNumEdges);
        _faces.resize(numFaces);
        
        for (auto i = 0; i < numVerts; i++)
        {
            _vertData[i].v = verts[i];
            _verts[i]._vertIdx = i;
        }
        
        std::map<std::pair<GenVert*, GenVert*>, GenEdge*> edgeMap;
        
        auto eIdx = 0;
        for (auto faceIdx = 0; faceIdx < numFaces; faceIdx++)
        {
            auto& faceIndices = faces[faceIdx];
            auto numVertsInFace = faces[faceIdx].size();
            
            // Vert._edgeIdx points to one of the half edges
            for (auto i = 0; i < numVertsInFace; i++)
            {
                _verts[faceIndices[i]]._edge = &_edges[eIdx];
            }
            
            // Edge._vertIdx points to vertex at end of half-edge
            for (auto i = 0, j = 1; i < numVertsInFace; i++, j++)
            {
                if (j == numVertsInFace)
                    j = 0;
                _edges[eIdx+i]._vert = &_verts[faceIndices[j]];
            }
            
            for (auto i = 0, j = 1; i < numVertsInFace; i++, j++)
            {
                if (j == numVertsInFace)
                    j = 0;
                edgeMap[std::make_pair(_edges[eIdx+i]._vert, _edges[eIdx+j]._vert)] = &_edges[eIdx+j];
            }
            
            // Edge._nextIdx points to the next edge round the face
            for (auto i = 0, j = 1; i < numVertsInFace; i++, j++)
            {
                if (j == numVertsInFace)
                    j = 0;
                _edges[eIdx+i]._next = &_edges[eIdx+j];
            }
            
            // Edge._nextIdx points to the previous edge round the face
            for (auto i = 0, j = 1; i < numVertsInFace; i++, j++)
            {
                if (j == numVertsInFace)
                    j = 0;
                _edges[eIdx+j]._prev = &_edges[eIdx+i];
            }
            
            // Edge._faceIdx all point to the same face
            for (auto i = 0; i < numVertsInFace; i++)
                _edges[eIdx+i]._face = &_faces[faceIdx];
            
            // Face._edgeIdx points to the first half-edge
            _faces[faceIdx]._edge = &_edges[eIdx];

            // Point to the first edge index for the next face
            eIdx += faces[faceIdx].size();
        }
        
        for (auto& edge: _edges)
        {
            auto vertTo = edge._vert;
            auto vertFrom = edge._prev->_vert;
            
            edge._opposite = edgeMap[std::make_pair(vertTo, vertFrom)];
        }
    }
    
    GenEdge* GenMesh::GetRelativeEdge(GenEdge* e, uint32_t offset) const
    {
        GenEdge* currEdge = e;
        for (auto i = 0; i < offset; i++)
            currEdge = currEdge->_next;
        return currEdge;
    }

    uint32_t GenMesh::GetNumEdgesInFace(const GenFace* f) const
    {
        uint32_t numEdges = 1;
        GenEdge* startEdge = f->_edge;
        GenEdge* currEdge = startEdge->_next;
        
        while (currEdge != startEdge)
        {
            currEdge = currEdge->_next;
            numEdges++;
        }
        
        return numEdges;
    }

    const GenVertData* GenMesh::GetVertData(const GenEdge* e) const
    {
        return &_vertData[e->_vert->_vertIdx];
    }

    Vec3f GenMesh::GetFaceNormal(const GenFace* f) const
    {
        auto a = _vertData[f->_edge->_vert->_vertIdx].v;
        auto b = _vertData[f->_edge->_next->_vert->_vertIdx].v;
        auto c = _vertData[f->_edge->_next->_next->_vert->_vertIdx].v;
        return Normal(a, b, c);
    }
}
