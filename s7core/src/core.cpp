#include "core.h"

#include <cmath>

namespace s7 {

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

}
