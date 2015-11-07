#pragma once

#include <array>

#include "core.h"

namespace s7 {

    struct Mat44
    {
        std::array<float, 16> _m;
        
        Mat44();
        Mat44(float e00, float e01, float e02, float e03,
              float e10, float e11, float e12, float e13,
              float e20, float e21, float e22, float e23,
              float e30, float e31, float e32, float e33);

        static Mat44 CreateIdentity();
        
        
        const float* Data() const;
        
        void Set(float e00, float e01, float e02, float e03,
                 float e10, float e11, float e12, float e13,
                 float e20, float e21, float e22, float e23,
                 float e30, float e31, float e32, float e33);
        
        void RotateXY(float x, float y);
    
        void LookAt(const Vec3f& eyePos, const Vec3f& target, const Vec3f& upDir);
        
        void PerspectiveProjection(
            float fovY,
            float aspectRatio,
            float near,
            float far);
    };

}
