#pragma once

#include <cstdint>

namespace s7 {

	struct Vec3f
	{
		float _x, _y, _z;

		Vec3f() : _x(0), _y(0), _z(0) {}
		Vec3f(float x, float y, float z) : _x(x), _y(y), _z(z) {}

		void Set(float x, float y, float z) { _x = x, _y = y, _z = z; }

		Vec3f operator+(const Vec3f& v) const { return Vec3f(_x + v._x, _y + v._y, _z + v._z); }
		Vec3f operator-(const Vec3f& v) const { return Vec3f(_x - v._x, _y - v._y, _z - v._z); }
		Vec3f operator*(const Vec3f& v) const { return Vec3f(_x * v._x, _y * v._y, _z * v._z); }
		Vec3f operator/(const Vec3f& v) const { return Vec3f(_x / v._x, _y / v._y, _z / v._z); }

		Vec3f operator+(float a) const { return Vec3f(_x + a, _y + a, _z + a); }
		Vec3f operator-(float a) const { return Vec3f(_x - a, _y - a, _z - a); }
		Vec3f operator*(float a) const { return Vec3f(_x * a, _y * a, _z * a); }
		Vec3f operator/(float a) const { return Vec3f(_x / a, _y / a, _z / a); }
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
	};


	extern float Sqrt(float f);

	extern float Distance(const Vec3f& v1, const Vec3f& v2);

	extern Aabb CreateAabbFromPoints(const Vec3f* pts, size_t numPts);
	extern Sphere CreateSphereFromAabb(const Aabb& box);
}
