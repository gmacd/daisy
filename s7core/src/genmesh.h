#pragma once

#include <initializer_list>
#include <vector>
#include "core.h"

namespace s7 {

    // TODO Separate vert details from GenVert?
    // TODO Hide support structs
    struct GenEdge;
    struct GenFace;

    struct GenVert
    {
        Vec3f v;
        GenEdge* _edge;
    };


    struct GenEdge
    {
        GenVert* _vert;
        GenEdge* _opposite;
        GenEdge* _prev;
        GenEdge* _next;
        GenFace* _face;
    };
    
    
    struct GenFace
    {
        GenEdge* _edge;
    };
    
    
    class GenMesh
    {
    public:
        GenMesh(uint32_t numVerts, uint32_t numEdges, uint32_t numFaces);
        GenMesh(
            std::vector<Vec3f> verts,
            std::vector<std::vector<uint32_t>> faces);
        
    private:
        std::vector<GenVert> _verts;
        std::vector<GenEdge> _edges;
        std::vector<GenFace> _faces;

        GenEdge* GetRelativeEdge(GenEdge* e, uint32_t offset) const;
        uint32_t GetNumEdgesInFace(GenFace* f) const;
    };

}
