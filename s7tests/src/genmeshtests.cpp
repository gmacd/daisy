#include "catch.hpp"

#include "genmesh.h"
#include "primitives.h"

using namespace s7;

TEST_CASE("Can define cube", "[genmesh]")
{
    auto mesh = CreateCubeMesh();
    
    REQUIRE(mesh->_faces.size() == 6);
    REQUIRE(mesh->_verts.size() == 8);
    REQUIRE(mesh->_edges.size() == 24);
    
    // Check opposites all link for each edge
    for (auto& e: mesh->_edges)
    {
        REQUIRE(&e == e._opposite->_opposite);
    }
    
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
    REQUIRE(mesh->_edges.size() == numEdgesBefore+1);
    REQUIRE(mesh->_faces.size() == numFacesBefore);
}
