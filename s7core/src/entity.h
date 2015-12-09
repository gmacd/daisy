#pragma once

namespace s7 {

    class UpdateContext;
    
    class Entity
    {
    public:
        void Init();
        void Update(const UpdateContext& ctx);
    };
    
}