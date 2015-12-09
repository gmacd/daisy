#include "entity.h"

#include "genmeshconverter.h"
#include "primitives.h"
#include "material.h"

namespace s7 {

    void Entity::Init()
    {
        auto program = LoadProgram("mesh.vert", "mesh.frag");
        
        auto genmesh = CreateOctahedronMesh();
        auto mesh = Convert(*genmesh);
        
    
    }
    
    void Entity::Update(const UpdateContext& ctx)
    {
        
    }
    
}
