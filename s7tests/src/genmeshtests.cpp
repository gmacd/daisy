#include "catch.hpp"

#include "genmesh.h"
#include "primitives.h"

using namespace s7;

void RequireGenMeshWellFormed(const GenMesh* mesh)
{
    // Verts
    REQUIRE(mesh->_verts.size() == mesh->_vertData.size());
    for (auto& v: mesh->_verts)
    {
        REQUIRE(v._vertIdx >= 0);
        REQUIRE(v._vertIdx < mesh->_vertData.size());
        REQUIRE(v._edge != nullptr);
    }
    
    // Edges
    for (auto& e: mesh->_edges)
    {
        REQUIRE(e._vert != nullptr);
        REQUIRE(e._opposite != nullptr);
        REQUIRE(e._prev != nullptr);
        REQUIRE(e._next != nullptr);
        REQUIRE(e._face != nullptr);
        
        REQUIRE(e._opposite->_opposite == &e);
        REQUIRE(e._next->_prev == &e);
    }
    
    // Faces
    for (auto& f: mesh->_faces)
    {
        REQUIRE(f._edge != nullptr);
        
        auto numEdgesInFace = mesh->GetNumEdgesInFace(&f);
        REQUIRE(f._edge == mesh->GetRelativeEdge(f._edge, numEdgesInFace));
    }
}

TEST_CASE("Can define cube", "[genmesh]")
{
    auto mesh = CreateCubeMesh();
    
    REQUIRE(mesh->_faces.size() == 6);
    REQUIRE(mesh->_verts.size() == 8);
    REQUIRE(mesh->_edges.size() == 24);
    
    // Check GetRelativeEdge
    for (auto& e: mesh->_edges)
    {
        REQUIRE(&e == mesh->GetRelativeEdge(&e, 4));
    }
    
    // Check number of edges in each face
    for (auto& f: mesh->_faces)
    {
        REQUIRE(mesh->GetNumEdgesInFace(&f) == 4);
    }
    
    RequireGenMeshWellFormed(mesh);
}

TEST_CASE("Can split edge", "[genmesh]")
{
    auto mesh = CreateOctahedronMesh();
    
    auto numVertsBefore = mesh->_verts.size();
    auto numEdgesBefore = mesh->_edges.size();
    auto numFacesBefore = mesh->_faces.size();
    
    mesh->SplitEdge(&mesh->_edges.front());
    
    REQUIRE(mesh->_vertData.size() == numVertsBefore+1);
    REQUIRE(mesh->_verts.size() == numVertsBefore+1);
    REQUIRE(mesh->_edges.size() == numEdgesBefore+2);
    REQUIRE(mesh->_faces.size() == numFacesBefore);
    
    RequireGenMeshWellFormed(mesh);
}
