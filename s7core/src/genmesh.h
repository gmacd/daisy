#pragma once

#include <initializer_list>
#include <vector>
#include "core.h"

namespace s7 {

    // TODO Keep GenVertData and GenVert separate or merge?
    // TODO Hide support structs
    // TODO Custom container that doesn't invalidate existing pointers
    struct GenEdge;
    struct GenFace;
    
    struct GenVertData
    {
        Vec3f v;
    };


    struct GenVert
    {
        int _vertIdx;
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
        // TODO Make these 4 private
        std::vector<GenVertData> _vertData;
        std::vector<GenVert> _verts;
        std::vector<GenEdge> _edges;
        std::vector<GenFace> _faces;


        GenMesh(uint32_t numVerts, uint32_t numEdges, uint32_t numFaces);
        GenMesh(
            std::vector<Vec3f> verts,
            std::vector<std::vector<uint32_t>> faces);
        

        GenEdge* GetRelativeEdge(GenEdge* e, uint32_t offset) const;
        uint32_t GetNumEdgesInFace(const GenFace* f) const;
        const GenVertData* GetVertData(const GenEdge* e) const;
        
        Vec3f GetFaceNormal(const GenFace* f) const;

        // Split an edge in two, returning the new vert.
        // Results in one new vert and two new edges.
        GenVert* SplitEdge(GenEdge* existingEdge);
    };

}
