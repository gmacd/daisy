#include "catch.hpp"

#include "genmesh.h"

using namespace s7;

TEST_CASE("Can define cube", "[genmesh]") {
    GenMesh mesh(
    {
        // Front
        Vec3f(-1, -1,  1),
        Vec3f( 1, -1,  1),
        Vec3f( 1,  1,  1),
        Vec3f(-1,  1,  1),
        // Back
        Vec3f(-1, -1, -1),
        Vec3f( 1, -1, -1),
        Vec3f( 1,  1, -1),
        Vec3f(-1,  1, -1)
    },
    {
        {0, 1, 2, 3},  // Front
        {3, 2, 6, 7},  // Top
        {7, 6, 5, 4},  // Back
        {4, 5, 1, 0},  // Bottom
        {4, 0, 3, 7},  // Left
        {1, 5, 6, 2},  // Right
    });
    
    REQUIRE(mesh._faces.size() == 6);
    REQUIRE(mesh._verts.size() == 8);
    REQUIRE(mesh._edges.size() == 24);
    
    // Check opposites all link for each edge
    for (auto& e: mesh._edges)
    {
        REQUIRE(&e == e._opposite->_opposite);
    }
    
    // Check GetRelativeEdge
    for (auto& e: mesh._edges)
    {
        REQUIRE(&e == mesh.GetRelativeEdge(&e, 4));
    }
    
    // Check number of edges in each face
    for (auto& f: mesh._faces)
    {
        REQUIRE(mesh.GetNumEdgesInFace(&f) == 4);
    }
}

