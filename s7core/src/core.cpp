#include "core.h"

#include <cmath>
#include <algorithm>

namespace s7 {

	float toRad(float deg)
	{
        return deg * Pi / 180.0f;
	}

	float toDeg(float rad)
	{
		return rad * 180.0f / Pi;
	}

    float Vec3f::Len() const
    {
        return Sqrt(_x*_x + _y*_y + _z*_z);
    }

    Vec3f& Vec3f::Normalize()
    {
        auto len = Len();
        _x /= len;
        _y /= len;
        _z /= len;
        return *this;
    }
    
    Vec3f Vec3f::Normal() const
    {
        auto len = Len();
        return Vec3f(_x/len, _y/len, _z/len);
    }
    
    float Vec3f::Dot(const Vec3f& v) const
    {
        return _x*v._x + _y*v._y + _z*v._z;
    }

    Vec3f Vec3f::Cross(const Vec3f& v) const
    {
        return Vec3f(
            _y*v._z - _z*v._y,
            _z*v._x - _x*v._z,
            _x*v._y - _y*v._x
        );
    }

    Vec3f Vec3f::MidPoint(const Vec3f& v) const
    {
        return Vec3f(
            (_x + v._x) / 2.0f,
            (_y + v._y) / 2.0f,
            (_z + v._z) / 2.0f);
    }

    const Vec3f Vec3f::XAxis(1, 0, 0);
    const Vec3f Vec3f::YAxis(0, 1, 0);
    const Vec3f Vec3f::ZAxis(0, 0, 1);

    // Direction is not normalized
    Ray Ray::RayFromPoints(const Vec3f& origin, const Vec3f& other)
    {
        return Ray(
            origin,
            other-origin);
    }

    float LineSegment::Len() const
    {
        return (_a-_b).Len();
    }

    Vec3f LineSegment::MidPoint() const
    {
        return _a.MidPoint(_b);
    }

    
    // TODO Improve this.
    float AlmostEquals(float a, float b)
    {
        return std::abs(a-b) <= 0.0001f;
    }

	float Sqrt(float f)
	{
		return std::sqrt(f);
	}
	
	float Distance(const Vec3f& v1, const Vec3f& v2)
	{
		auto a = v2 - v1;
		return Sqrt(a._x*a._x + a._y*a._y + a._z*a._z);
	}

	Aabb CreateAabbFromPoints(const Vec3f* pts, size_t numPts)
	{
		Aabb box(pts[0], pts[0]);

		for (size_t i = 1; i < numPts; i++)
		{
			auto& pt = pts[i];

			if      (pt._x < box._min._x) box._min._x = pt._x;
			else if (pt._x > box._max._x) box._max._x = pt._x;

			if      (pt._y < box._min._y) box._min._y = pt._y;
			else if (pt._y > box._max._y) box._max._y = pt._y;

			if      (pt._z < box._min._z) box._min._z = pt._z;
			else if (pt._z > box._max._z) box._max._z = pt._z;
		}

		return box;
	}

	Sphere CreateSphereFromAabb(const Aabb& box)
	{
		auto centre = box._min + ((box._max - box._min) / 2.0f);
		auto radius = Distance(centre, box._max);
		Sphere s(centre, radius);
		return s;
	}

    Sphere CreateSphereFromSpheres(const Sphere* spheres, size_t numSpheres)
    {
        Sphere maxSphere(spheres[0]._centre, spheres[0]._radius);
        
        for (auto i = 1; i < numSpheres; i++)
        {
            auto& s = spheres[i];
            
            // If sphere has same midpoint, check radius
            if (s._centre == maxSphere._centre)
            {
                maxSphere._radius = std::max(maxSphere._radius, s._radius);
                continue;
            }
            
            Line line(maxSphere._centre, s._centre);
            
            Vec3f pts[4];// s1p1, s1p2, s2p1, s2p2;
            Intersect(maxSphere, line, pts[0], pts[1]);
            Intersect(s, line, pts[2], pts[3]);
            
            // Now find 2 furthest points on the line
            LineSegment longestLine(line._a, line._b);
            float longestLineLen = longestLine.Len();
            for (auto pa = 0; pa < 3; pa++)
            {
                for (auto pb = pa+1; pb < 4; pb++)
                {
                    LineSegment currLine(pts[pa], pts[pb]);
                    float currLineLen = currLine.Len();
                    if (currLineLen > longestLineLen)
                    {
                        longestLine = currLine;
                        longestLineLen = currLineLen;
                    }
                }
            }
            
            // maxSphere is now the sphere centred on longestLine,
            // encompassing both end points.
            maxSphere.Set(longestLine.MidPoint(), longestLineLen/2.0f);
        }
        
        return maxSphere;
    }

    bool Intersect(const Sphere& s, const Line& l, Vec3f& i1, Vec3f& i2)
    {
        // http://paulbourke.net/geometry/circlesphere/index.html#linesphere
        float a = (l._b._x - l._a._x) * (l._b._x - l._a._x)
                + (l._b._y - l._a._y) * (l._b._y - l._a._y)
                + (l._b._z - l._a._z) * (l._b._z - l._a._z);
        float b = 2.0f * (((l._b._x - l._a._x) * (l._a._x - s._centre._x))
                        + ((l._b._y - l._a._y) * (l._a._y - s._centre._y))
                        + ((l._b._z - l._a._z) * (l._a._z - s._centre._z)));
        float c = s._centre._x * s._centre._x
                + s._centre._y * s._centre._y
                + s._centre._z * s._centre._z
                + l._a._x * l._a._x
                + l._a._y * l._a._y
                + l._a._z * l._a._z
                - 2.0f * (s._centre._x * l._a._x
                          + s._centre._y * l._a._y
                          + s._centre._z * l._a._z)
                - s._radius * s._radius;
        
        float discriminant = b*b - 4.0f*a*c;
        if (discriminant < 0.0f)
        {
            // No intersection
            return false;
        }
        else if (discriminant == 0.0f)
        {
            // Tangential intersection - one point
            float u = -b/(2.0f*a);
            i1 = l._a + (u * (l._b - l._a));
            return true;
        }
        else
        {
            // Straight through intersection - two points
            float sqrtDisc = Sqrt(discriminant);
            float u1 = (-b + sqrtDisc) / (2.0f * a);
            float u2 = (-b - sqrtDisc) / (2.0f * a);
            i1 = l._a + (u1 * (l._b - l._a));
            i2 = l._a + (u2 * (l._b - l._a));
            return true;
        }
    }
}
