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
        auto numVertsInFace = (uint32_t)(*faces.begin()).size();
        
        _vertData.resize(numVerts);
        _verts.resize(numVerts);
        _edges.resize(numFaces * numVertsInFace);
        _faces.resize(numFaces);
        
        for (auto i = 0; i < numVerts; i++)
        {
            _vertData[i].v = verts[i];
            _verts[i]._vertIdx = i;
        }
        
        std::map<std::pair<GenVert*, GenVert*>, GenEdge*> edgeMap;
        
        // TODO Use loop 0..numVertsInFace in assignments below
        //      (to allow arbitrary sized faces)
        for (auto faceIdx = 0; faceIdx < numFaces; faceIdx++)
        {
            auto& faceIndices = faces[faceIdx];
            auto eIdx = faceIdx * numVertsInFace;
            
            // Vert._edgeIdx points to one of the half edges
            _verts[faceIndices[0]]._edge = &_edges[eIdx];
            _verts[faceIndices[1]]._edge = &_edges[eIdx];
            _verts[faceIndices[2]]._edge = &_edges[eIdx];
            _verts[faceIndices[3]]._edge = &_edges[eIdx];
            
            // Edge._vertIdx points to vertex at end of half-edge
            _edges[eIdx  ]._vert = &_verts[faceIndices[1]];
            _edges[eIdx+1]._vert = &_verts[faceIndices[2]];
            _edges[eIdx+2]._vert = &_verts[faceIndices[3]];
            _edges[eIdx+3]._vert = &_verts[faceIndices[0]];
            
            edgeMap[std::make_pair(_edges[eIdx+3]._vert, _edges[eIdx  ]._vert)] = &_edges[eIdx];
            edgeMap[std::make_pair(_edges[eIdx  ]._vert, _edges[eIdx+1]._vert)] = &_edges[eIdx+1];
            edgeMap[std::make_pair(_edges[eIdx+1]._vert, _edges[eIdx+2]._vert)] = &_edges[eIdx+2];
            edgeMap[std::make_pair(_edges[eIdx+2]._vert, _edges[eIdx+3]._vert)] = &_edges[eIdx+3];
            
            // Edge._nextIdx points to the next edge round the face
            _edges[eIdx  ]._next = &_edges[eIdx+1];
            _edges[eIdx+1]._next = &_edges[eIdx+2];
            _edges[eIdx+2]._next = &_edges[eIdx+3];
            _edges[eIdx+3]._next = &_edges[eIdx];
            
            // Edge._nextIdx points to the previous edge round the face
            _edges[eIdx  ]._prev = &_edges[eIdx+3];
            _edges[eIdx+1]._prev = &_edges[eIdx];
            _edges[eIdx+2]._prev = &_edges[eIdx+1];
            _edges[eIdx+3]._prev = &_edges[eIdx+2];
            
            // Edge._faceIdx all point to the same face
            _edges[eIdx  ]._face = &_faces[faceIdx];
            _edges[eIdx+1]._face = &_faces[faceIdx];
            _edges[eIdx+2]._face = &_faces[faceIdx];
            _edges[eIdx+3]._face = &_faces[faceIdx];
            
            // Face._edgeIdx points to the first half-edge
            _faces[faceIdx]._edge = &_edges[eIdx];
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
