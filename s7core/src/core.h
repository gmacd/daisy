#pragma once

#include <cstdint>

namespace s7 {

	struct Vec3f
	{
		float _x, _y, _z;

		Vec3f() : _x(0), _y(0), _z(0) {}
		Vec3f(float x, float y, float z) : _x(x), _y(y), _z(z) {}

		void Set(float x, float y, float z) { _x = x, _y = y, _z = z; }
        
        bool operator==(const Vec3f& v) const { return this == &v || (_x == v._x && _y == v._y && _z == v._z); }
        
		Vec3f operator+(const Vec3f& v) const { return Vec3f(_x + v._x, _y + v._y, _z + v._z); }
		Vec3f operator-(const Vec3f& v) const { return Vec3f(_x - v._x, _y - v._y, _z - v._z); }
		Vec3f operator*(const Vec3f& v) const { return Vec3f(_x * v._x, _y * v._y, _z * v._z); }
		Vec3f operator/(const Vec3f& v) const { return Vec3f(_x / v._x, _y / v._y, _z / v._z); }

		Vec3f operator+(float a) const { return Vec3f(_x + a, _y + a, _z + a); }
		Vec3f operator-(float a) const { return Vec3f(_x - a, _y - a, _z - a); }
		Vec3f operator*(float a) const { return Vec3f(_x * a, _y * a, _z * a); }
		Vec3f operator/(float a) const { return Vec3f(_x / a, _y / a, _z / a); }
        
        friend Vec3f operator+(float a, const Vec3f& v) { return Vec3f(a + v._x, a + v._y, a + v._z); }
        friend Vec3f operator-(float a, const Vec3f& v) { return Vec3f(a - v._x, a - v._y, a - v._z); }
        friend Vec3f operator*(float a, const Vec3f& v) { return Vec3f(a * v._x, a * v._y, a * v._z); }
        
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
		uint8_t _x, _y, _z, _w;

		Vec4u8() : _x(0), _y(0), _z(0), _w(0) {}
		Vec4u8(uint8_t x, uint8_t y, uint8_t z, uint8_t w) : _x(x), _y(y), _z(z), _w(w) {}

		void Set(uint8_t x, uint8_t y, uint8_t z, uint8_t w) { _x = x, _y = y, _z = z, _w = w; }

		Vec4u8 operator+(const Vec4u8& v) const { return Vec4u8(_x + v._x, _y + v._y, _z + v._z, _w + v._w); }
		Vec4u8 operator-(const Vec4u8& v) const { return Vec4u8(_x - v._x, _y - v._y, _z - v._z, _w - v._w); }
		Vec4u8 operator*(const Vec4u8& v) const { return Vec4u8(_x * v._x, _y * v._y, _z * v._z, _w * v._w); }
		Vec4u8 operator/(const Vec4u8& v) const { return Vec4u8(_x / v._x, _y / v._y, _z / v._z, _w / v._w); }

		Vec4u8 operator+(uint8_t a) const { return Vec4u8(_x + a, _y + a, _z + a, _w + a); }
		Vec4u8 operator-(uint8_t a) const { return Vec4u8(_x - a, _y - a, _z - a, _w - a); }
		Vec4u8 operator*(uint8_t a) const { return Vec4u8(_x * a, _y * a, _z * a, _w * a); }
		Vec4u8 operator/(uint8_t a) const { return Vec4u8(_x / a, _y / a, _z / a, _w / a); }
	};
    
    
    struct Ray
    {
        Vec3f _origin, _dir;
        
        Ray(const Vec3f& origin, const Vec3f& dir): _origin(origin), _dir(dir) {}
        
        static Ray RayFromPoints(const Vec3f& origin, const Vec3f& other);
    };
    
    
    // Infinite line
    struct Line
    {
        Vec3f _a, _b;
        
        Line(const Vec3f& a, const Vec3f& b): _a(a), _b(b) {}
    };

    
    struct LineSegment
    {
        Vec3f _a, _b;
        
        LineSegment(const Vec3f& a, const Vec3f& b): _a(a), _b(b) {}
        
        float Len() const;
        Vec3f MidPoint() const;
    };

    
	struct Matrix44
	{
		float _m[16];
	};


	struct Aabb
	{
		Vec3f _min;
		Vec3f _max;

		Aabb(): _min(), _max() {}
		Aabb(const Vec3f& min, const Vec3f& max) : _min(min), _max(max) {}
	};


	struct Obb
	{
		Matrix44 _m;
	};


	struct Sphere
	{
		Vec3f _centre;
		float _radius;

		Sphere() : _centre(), _radius(0.0f) {}
		Sphere(const Vec3f& c, float r) : _centre(c), _radius(r) {}
        Sphere(float x, float y, float z, float r) : _centre(x, y, z), _radius(r) {}

        void Set(const Vec3f& c, float r) { _centre = c, _radius = r; }
    };
    
    static const float Pi     = 3.14159265358979323846f;
	static const float InvPi  = 1.0f / 3.14159265358979323846f;
	static const float PiHalf = 1.57079632679489661923f;

	extern float toRad(float deg);
	extern float toDeg(float rad);

    extern float AlmostEquals(float a, float b);
	extern float Sqrt(float f);
	extern float Distance(const Vec3f& v1, const Vec3f& v2);

	extern Aabb CreateAabbFromPoints(const Vec3f* pts, size_t numPts);
	extern Sphere CreateSphereFromAabb(const Aabb& box);
    extern Sphere CreateSphereFromSpheres(const Sphere* spheres, size_t numSpheres);
    
    // Return true if the sphere intrescts the line.  If so, i1 and possiblly
    // i2 will be the intersection points.
    extern bool Intersect(const Sphere& s, const Line& l, Vec3f& i1, Vec3f& i2);
}
