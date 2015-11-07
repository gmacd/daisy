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
        return Sqrt(x*x + y*y + z*z);
    }

    Vec3f& Vec3f::Normalize()
    {
        auto len = Len();
        x /= len;
        y /= len;
        z /= len;
        return *this;
    }
    
    Vec3f Vec3f::Normal() const
    {
        auto len = Len();
        return Vec3f(x/len, y/len, z/len);
    }
    
    float Vec3f::Dot(const Vec3f& v) const
    {
        return x*v.x + y*v.y + z*v.z;
    }

    Vec3f Vec3f::Cross(const Vec3f& v) const
    {
        return Vec3f(
            y*v.z - z*v.y,
            z*v.x - x*v.z,
            x*v.y - y*v.x
        );
    }

    Vec3f Vec3f::MidPoint(const Vec3f& v) const
    {
        return Vec3f(
            (x + v.x) / 2.0f,
            (y + v.y) / 2.0f,
            (z + v.z) / 2.0f);
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
		return Sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
	}

	Aabb CreateAabbFromPoints(const Vec3f* pts, size_t numPts)
	{
		Aabb box(pts[0], pts[0]);

		for (size_t i = 1; i < numPts; i++)
		{
			auto& pt = pts[i];

			if      (pt.x < box._min.x) box._min.x = pt.x;
			else if (pt.x > box._max.x) box._max.x = pt.x;

			if      (pt.y < box._min.y) box._min.y = pt.y;
			else if (pt.y > box._max.y) box._max.y = pt.y;

			if      (pt.z < box._min.z) box._min.z = pt.z;
			else if (pt.z > box._max.z) box._max.z = pt.z;
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
        float a = (l._b.x - l._a.x) * (l._b.x - l._a.x)
                + (l._b.y - l._a.y) * (l._b.y - l._a.y)
                + (l._b.z - l._a.z) * (l._b.z - l._a.z);
        float b = 2.0f * (((l._b.x - l._a.x) * (l._a.x - s._centre.x))
                        + ((l._b.y - l._a.y) * (l._a.y - s._centre.y))
                        + ((l._b.z - l._a.z) * (l._a.z - s._centre.z)));
        float c = s._centre.x * s._centre.x
                + s._centre.y * s._centre.y
                + s._centre.z * s._centre.z
                + l._a.x * l._a.x
                + l._a.y * l._a.y
                + l._a.z * l._a.z
                - 2.0f * (s._centre.x * l._a.x
                          + s._centre.y * l._a.y
                          + s._centre.z * l._a.z)
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
