#include "mat.h"

#include <cmath>
#include <algorithm>

namespace s7 {
    
    Mat44::Mat44()
    {
        m.fill(0.0f);
    }
    
    Mat44::Mat44(
        float e00, float e01, float e02, float e03,
        float e10, float e11, float e12, float e13,
        float e20, float e21, float e22, float e23,
        float e30, float e31, float e32, float e33)
    {
        m[ 0] = e00; m[ 1] = e01; m[ 2] = e02; m[ 3] = e03;
        m[ 4] = e10; m[ 5] = e11; m[ 6] = e12; m[ 7] = e13;
        m[ 8] = e20; m[ 9] = e21; m[10] = e22; m[11] = e23;
        m[12] = e30; m[13] = e31; m[14] = e32; m[15] = e33;
    }

    Mat44 Mat44::CreateIdentity()
    {
        return Mat44(
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1);
    }
    
    const float* Mat44::Data() const
    {
        return m.data();
    }
    
    void Mat44::Set(
        float e00, float e01, float e02, float e03,
        float e10, float e11, float e12, float e13,
        float e20, float e21, float e22, float e23,
        float e30, float e31, float e32, float e33)
    {
        m[ 0] = e00; m[ 1] = e01; m[ 2] = e02; m[ 3] = e03;
        m[ 4] = e10; m[ 5] = e11; m[ 6] = e12; m[ 7] = e13;
        m[ 8] = e20; m[ 9] = e21; m[10] = e22; m[11] = e23;
        m[12] = e30; m[13] = e31; m[14] = e32; m[15] = e33;
    }

    void Mat44::RotateXY(float x, float y)
	{
		auto sx = std::sin(x);
		auto cx = std::cos(x);
		auto sy = std::sin(y);
		auto cy = std::cos(y);
        
        Set(cy,     0,  sy,     0,
            sx*sy,  cx, -sx*cy, 0,
            -cx*sy, sx, cx*cy,  0,
            0,      0,  0,      1);
    }


    void Mat44::LookAt(const Vec3f& eye, const Vec3f& target, const Vec3f& upDir)
    {
        auto view = (target - eye).Normalize();
        auto right = upDir.Cross(view).Normalize();
        auto up = view.Cross(right);
        
        m[ 0] = right.x; m[ 1] = up.x; m[ 2] = view.x; m[ 3] = 0;
        m[ 4] = right.y; m[ 5] = up.y; m[ 6] = view.y; m[ 7] = 0;
        m[ 8] = right.z; m[ 9] = up.z; m[10] = view.z; m[11] = 0;
        m[12] = -right.Dot(eye);
        m[13] = -up.Dot(eye);
        m[14] = -view.Dot(eye);
        m[15] = 1;
    }

    void Mat44::PerspectiveProjection(
        float fovY,
        float aspectRatio,
        float near,
        float far)
    {
		auto height = 1.0f / std::tan(toRad(fovY) * 0.5f);
		auto width  = height * 1.0f / aspectRatio;
        auto diff   = far - near;
		auto aa     = far / diff;
		auto bb     = -near * aa;

        Set(width, 0,      0,  0,
            0,     height, 0,  0,
            0,     0,      aa, 1,
            0,     0,      bb, 0);
    }
}
