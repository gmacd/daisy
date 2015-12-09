#pragma once

#include <cstdint>

namespace s7 {

	struct Vec4f
	{
		float x, y, z, w;

		Vec4f() : x(0), y(0), z(0), w(0) {}
		Vec4f(float xx, float yy, float zz, float ww) : x(xx), y(yy), z(zz), w(ww) {}

		void Set(float xx, float yy, float zz, float ww) { x = xx, y = yy, z = zz, w = ww; }
        
        bool operator==(const Vec4f& v) const { return this == &v || (x == v.x && y == v.y && z == v.z && w == v.w); }
        
		Vec4f operator+(const Vec4f& v) const { return Vec4f(x + v.x, y + v.y, z + v.z, w + v.w); }
		Vec4f operator-(const Vec4f& v) const { return Vec4f(x - v.x, y - v.y, z - v.z, w - v.w); }
		Vec4f operator*(const Vec4f& v) const { return Vec4f(x * v.x, y * v.y, z * v.z, w * v.w); }
		Vec4f operator/(const Vec4f& v) const { return Vec4f(x / v.x, y / v.y, z / v.z, w / v.w); }

		Vec4f operator+(float a) const { return Vec4f(x + a, y + a, z + a, w + a); }
		Vec4f operator-(float a) const { return Vec4f(x - a, y - a, z - a, w - a); }
		Vec4f operator*(float a) const { return Vec4f(x * a, y * a, z * a, w * a); }
		Vec4f operator/(float a) const { return Vec4f(x / a, y / a, z / a, w / a); }
        
        friend Vec4f operator+(float a, const Vec4f& v) { return Vec4f(a + v.x, a + v.y, a + v.z, a + v.w); }
        friend Vec4f operator-(float a, const Vec4f& v) { return Vec4f(a - v.x, a - v.y, a - v.z, a - v.w); }
        friend Vec4f operator*(float a, const Vec4f& v) { return Vec4f(a * v.x, a * v.y, a * v.z, a * v.w); }
        
        Vec4f& Normalize();
        
        float Len() const;
        Vec4f Normal() const;
        float Dot(const Vec4f& v) const;
        Vec4f Cross(const Vec4f& v) const;
        
        Vec4f MidPoint(const Vec4f&v) const;
        
        static const Vec4f XAxis;
        static const Vec4f YAxis;
        static const Vec4f ZAxis;
	};


	struct Vec3f
	{
		float x, y, z;

		Vec3f() : x(0), y(0), z(0) {}
		Vec3f(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {}

		void Set(float xx, float yy, float zz) { x = xx, y = yy, z = zz; }
        
        bool operator==(const Vec3f& v) const { return this == &v || (x == v.x && y == v.y && z == v.z); }
        
		Vec3f operator+(const Vec3f& v) const { return Vec3f(x + v.x, y + v.y, z + v.z); }
		Vec3f operator-(const Vec3f& v) const { return Vec3f(x - v.x, y - v.y, z - v.z); }
		Vec3f operator*(const Vec3f& v) const { return Vec3f(x * v.x, y * v.y, z * v.z); }
		Vec3f operator/(const Vec3f& v) const { return Vec3f(x / v.x, y / v.y, z / v.z); }

		Vec3f operator+(float a) const { return Vec3f(x + a, y + a, z + a); }
		Vec3f operator-(float a) const { return Vec3f(x - a, y - a, z - a); }
		Vec3f operator*(float a) const { return Vec3f(x * a, y * a, z * a); }
		Vec3f operator/(float a) const { return Vec3f(x / a, y / a, z / a); }
        
        friend Vec3f operator+(float a, const Vec3f& v) { return Vec3f(a + v.x, a + v.y, a + v.z); }
        friend Vec3f operator-(float a, const Vec3f& v) { return Vec3f(a - v.x, a - v.y, a - v.z); }
        friend Vec3f operator*(float a, const Vec3f& v) { return Vec3f(a * v.x, a * v.y, a * v.z); }
        
        Vec3f& Normalize();
        
        float Len() const;
        Vec3f Normal() const;
        float Dot(const Vec3f& v) const;
        Vec3f Cross(const Vec3f& v) const;
        
        Vec3f MidPoint(const Vec3f&v) const;
        
        static const Vec3f XAxis;
        static const Vec3f YAxis;
        static const Vec3f ZAxis;
	};


	struct Vec4u8
	{
		uint8_t x, y, z, w;

		Vec4u8() : x(0), y(0), z(0), w(0) {}
		Vec4u8(uint8_t xx, uint8_t yy, uint8_t zz, uint8_t ww) : x(xx), y(yy), z(zz), w(ww) {}

		void Set(uint8_t xx, uint8_t yy, uint8_t zz, uint8_t ww) { x = xx, y = yy, z = zz, w = ww; }
        
        Vec4u8& operator=(const Vec3f& v);

		Vec4u8 operator+(const Vec4u8& v) const { return Vec4u8(x + v.x, y + v.y, z + v.z, w + v.w); }
		Vec4u8 operator-(const Vec4u8& v) const { return Vec4u8(x - v.x, y - v.y, z - v.z, w - v.w); }
		Vec4u8 operator*(const Vec4u8& v) const { return Vec4u8(x * v.x, y * v.y, z * v.z, w * v.w); }
		Vec4u8 operator/(const Vec4u8& v) const { return Vec4u8(x / v.x, y / v.y, z / v.z, w / v.w); }

		Vec4u8 operator+(uint8_t a) const { return Vec4u8(x + a, y + a, z + a, w + a); }
		Vec4u8 operator-(uint8_t a) const { return Vec4u8(x - a, y - a, z - a, w - a); }
		Vec4u8 operator*(uint8_t a) const { return Vec4u8(x * a, y * a, z * a, w * a); }
		Vec4u8 operator/(uint8_t a) const { return Vec4u8(x / a, y / a, z / a, w / a); }
	};
    
    
    struct Ray
    {
        Vec3f _origin, _dir;
        
        Ray(const Vec3f& origin, const Vec3f& dir): _origin(origin), _dir(dir) {}
        
        static Ray RayFromPoints(const Vec3f& p1, const Vec3f& p2);
    };
    
    
    // Infinite line
    struct Line
    {
        Vec3f a, b;
        
        Line(const Vec3f& aa, const Vec3f& bb): a(aa), b(bb) {}
    };

    
    struct LineSegment
    {
        Vec3f a, b;
        
        LineSegment(const Vec3f& aa, const Vec3f& bb): a(aa), b(bb) {}
        
        float Len() const;
        Vec3f MidPoint() const;
    };

    
	struct Aabb
	{
		Vec3f min;
		Vec3f max;

		Aabb(): min(), max() {}
		Aabb(const Vec3f& mmin, const Vec3f& mmax) : min(mmin), max(mmax) {}
	};


	struct Sphere
	{
		Vec3f centre;
		float radius;

		Sphere() : centre(), radius(0.0f) {}
		Sphere(const Vec3f& c, float r) : centre(c), radius(r) {}
        Sphere(float x, float y, float z, float r) : centre(x, y, z), radius(r) {}

        void Set(const Vec3f& c, float r) { centre = c, radius = r; }
    };
    
    static const float Pi     = 3.14159265358979323846f;
	static const float InvPi  = 1.0f / 3.14159265358979323846f;
	static const float PiHalf = 1.57079632679489661923f;

	extern float toRad(float deg);
	extern float toDeg(float rad);

    extern float AlmostEquals(float a, float b);
	extern float Sqrt(float f);
	extern float Distance(const Vec3f& v1, const Vec3f& v2);
    
    extern Vec3f Normal(const Vec3f a, const Vec3f b, const Vec3f c);

	extern Aabb CreateAabbFromPoints(const Vec3f* pts, size_t numPts);
	extern Sphere CreateSphereFromAabb(const Aabb& box);
    extern Sphere CreateSphereFromSpheres(const Sphere* spheres, size_t numSpheres);
    
    // Return true if the sphere intrescts the line.  If so, i1 and possiblly
    // i2 will be the intersection points.
    extern bool Intersect(const Sphere& s, const Line& l, Vec3f& i1, Vec3f& i2);
}
