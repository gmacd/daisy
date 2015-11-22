#include "primitives.h"

#include "genmesh.h"

namespace s7 {

    GenMesh* CreateCubeMesh()
    {
        return new GenMesh(
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
    }

    GenMesh* CreateOctahedronMesh()
    {
        return new GenMesh(
        {
            Vec3f( 0,  1,  0), // top
            Vec3f( 0,  0,  1),
            Vec3f( 1,  0,  0),
            Vec3f(-1,  0,  0),
            Vec3f( 0,  0, -1),
            Vec3f( 0, -1,  0), // bottom
        },
        {
            {0, 1, 2},
            {0, 2, 4},
            {0, 4, 3},
            {0, 3, 1},
            {5, 2, 1},
            {5, 4, 2},
            {5, 3, 4},
            {5, 1, 3},
        });
    }

};
