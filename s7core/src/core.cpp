#include "core.h"

#include <cmath>
#include <algorithm>


// TODO - Point3, and review all vec* code?

namespace s7 {

	float toRad(float deg)
	{
        return deg * Pi / 180.0f;
	}

	float toDeg(float rad)
	{
		return rad * 180.0f / Pi;
	}


    ///////////////////////////////////////////
    // Vec3f


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


    ///////////////////////////////////////////
    // Vec4f


    float Vec4f::Len() const
    {
        return Sqrt(x*x + y*y + z*z + w*w);
    }

    Vec4f& Vec4f::Normalize()
    {
        auto len = Len();
        x /= len;
        y /= len;
        z /= len;
        w /= len;
        return *this;
    }
    
    Vec4f Vec4f::Normal() const
    {
        auto len = Len();
        return Vec4f(x/len, y/len, z/len, w/len);
    }
    
    float Vec4f::Dot(const Vec4f& v) const
    {
        return x*v.x + y*v.y + z*v.z + w*v.w;
    }

    Vec4f Vec4f::Cross(const Vec4f& v) const
    {
        return Vec4f(
            y*v.z - z*v.y,
            z*v.x - x*v.z,
            x*v.y - y*v.x,
            1.0f
        );
    }

    Vec4f Vec4f::MidPoint(const Vec4f& v) const
    {
        return Vec4f(
            (x + v.x) / 2.0f,
            (y + v.y) / 2.0f,
            (z + v.z) / 2.0f,
            0.0f);
    }

    const Vec4f Vec4f::XAxis(1, 0, 0, 1);
    const Vec4f Vec4f::YAxis(0, 1, 0, 1);
    const Vec4f Vec4f::ZAxis(0, 0, 1, 1);
    
    
    ///////////////////////////////////////////
    // Vec4u8


    Vec4u8& Vec4u8::operator=(const Vec3f& v)
    {
        x = (uint8_t)(255.0f/v.x);
        y = (uint8_t)(255.0f/v.y);
        z = (uint8_t)(255.0f/v.z);
        w = 1.0f;
        return *this;
    }


    ///////////////////////////////////////////
    // Ray


    // Direction is not normalized
    Ray Ray::RayFromPoints(const Vec3f& p1, const Vec3f& p2)
    {
        return Ray(p1, p2-p1);
    }

    float LineSegment::Len() const
    {
        return (a-b).Len();
    }

    Vec3f LineSegment::MidPoint() const
    {
        return a.MidPoint(b);
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
    
    Vec3f Normal(const Vec3f a, const Vec3f b, const Vec3f c)
    {
        return (b-a).Cross(c-a).Normal();
    }


	Aabb CreateAabbFromPoints(const Vec3f* pts, size_t numPts)
	{
		Aabb box(pts[0], pts[0]);

		for (size_t i = 1; i < numPts; i++)
		{
			auto& pt = pts[i];

			if      (pt.x < box.min.x) box.min.x = pt.x;
			else if (pt.x > box.max.x) box.max.x = pt.x;

			if      (pt.y < box.min.y) box.min.y = pt.y;
			else if (pt.y > box.max.y) box.max.y = pt.y;

			if      (pt.z < box.min.z) box.min.z = pt.z;
			else if (pt.z > box.max.z) box.max.z = pt.z;
		}

		return box;
	}

	Sphere CreateSphereFromAabb(const Aabb& box)
	{
		auto centre = box.min + ((box.max - box.min) / 2.0f);
		auto radius = Distance(centre, box.max);
		Sphere s(centre, radius);
		return s;
	}

    Sphere CreateSphereFromSpheres(const Sphere* spheres, size_t numSpheres)
    {
        Sphere maxSphere(spheres[0].centre, spheres[0].radius);
        
        for (auto i = 1; i < numSpheres; i++)
        {
            auto& s = spheres[i];
            
            // If sphere has same midpoint, check radius
            if (s.centre == maxSphere.centre)
            {
                maxSphere.radius = std::max(maxSphere.radius, s.radius);
                continue;
            }
            
            Line line(maxSphere.centre, s.centre);
            
            Vec3f pts[4];// s1p1, s1p2, s2p1, s2p2;
            Intersect(maxSphere, line, pts[0], pts[1]);
            Intersect(s, line, pts[2], pts[3]);
            
            // Now find 2 furthest points on the line
            LineSegment longestLine(line.a, line.b);
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
        float a = (l.b.x - l.a.x) * (l.b.x - l.a.x)
                + (l.b.y - l.a.y) * (l.b.y - l.a.y)
                + (l.b.z - l.a.z) * (l.b.z - l.a.z);
        float b = 2.0f * (((l.b.x - l.a.x) * (l.a.x - s.centre.x))
                        + ((l.b.y - l.a.y) * (l.a.y - s.centre.y))
                        + ((l.b.z - l.a.z) * (l.a.z - s.centre.z)));
        float c = s.centre.x * s.centre.x
                + s.centre.y * s.centre.y
                + s.centre.z * s.centre.z
                + l.a.x * l.a.x
                + l.a.y * l.a.y
                + l.a.z * l.a.z
                - 2.0f * (s.centre.x * l.a.x
                          + s.centre.y * l.a.y
                          + s.centre.z * l.a.z)
                - s.radius * s.radius;
        
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
            i1 = l.a + (u * (l.b - l.a));
            return true;
        }
        else
        {
            // Straight through intersection - two points
            float sqrtDisc = Sqrt(discriminant);
            float u1 = (-b + sqrtDisc) / (2.0f * a);
            float u2 = (-b - sqrtDisc) / (2.0f * a);
            i1 = l.a + (u1 * (l.b - l.a));
            i2 = l.a + (u2 * (l.b - l.a));
            return true;
        }
    }
}
