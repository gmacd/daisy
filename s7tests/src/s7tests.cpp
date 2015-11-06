#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>

#include "core.h"

using namespace s7;

TEST_CASE("Misc maths functions", "[maths]") {
    REQUIRE(Sqrt(25) == 5);
}

TEST_CASE("Distances are correct", "[maths]") {
    REQUIRE(AlmostEquals(Distance(Vec3f(0, 0, 0), Vec3f(5, 5, 5)), 8.6603f));
}

TEST_CASE("Can create AABB from points", "[maths]") {
    Vec3f pts1[] = { Vec3f(0, 0, 0) };
    auto aabb = CreateAabbFromPoints(pts1, 1);
    REQUIRE(aabb._min == Vec3f(0, 0, 0));
    REQUIRE(aabb._max == Vec3f(0, 0, 0));

    Vec3f pts2[] = { Vec3f(-1, -1, -1), Vec3f(1, 1, 1) };
    aabb = CreateAabbFromPoints(pts2, 2);
    REQUIRE(aabb._min == Vec3f(-1, -1, -1));
    REQUIRE(aabb._max == Vec3f(1, 1, 1));
    
    Vec3f pts3[] = { Vec3f(1, 1, 1), Vec3f(-1, -1, -1) };
    aabb = CreateAabbFromPoints(pts3, 2);
    REQUIRE(aabb._min == Vec3f(-1, -1, -1));
    REQUIRE(aabb._max == Vec3f(1, 1, 1));
}

TEST_CASE("Can create bounding sphere from spheres", "[maths]") {
    Sphere s1(0, 0, 0, 10);
    Sphere s2(0, 0, 0, 20);
    Sphere s3(10, 0, 0, 10);
    Sphere s4(20, 0, 0, 10);
    
    Sphere spheres1[] = {s1};
    auto rs = CreateSphereFromSpheres(spheres1, 1);
    REQUIRE(rs._centre == Vec3f(0, 0, 0));
    REQUIRE(rs._radius == 10);
    
    Sphere spheres2[] = {s1, s1};
    rs = CreateSphereFromSpheres(spheres2, 2);
    REQUIRE(rs._centre == Vec3f(0, 0, 0));
    REQUIRE(rs._radius == 10);
    
    Sphere spheres3[] = {s1, s2};
    rs = CreateSphereFromSpheres(spheres3, 2);
    REQUIRE(rs._centre == Vec3f(0, 0, 0));
    REQUIRE(rs._radius == 20);
    
    Sphere spheres4[] = {s1, s3};
    rs = CreateSphereFromSpheres(spheres4, 2);

    REQUIRE(rs._centre == Vec3f(5, 0, 0));
    REQUIRE(rs._radius == 15);
    
    Sphere spheres5[] = {s1, s4};
    rs = CreateSphereFromSpheres(spheres5, 2);
    REQUIRE(rs._centre == Vec3f(10, 0, 0));
    REQUIRE(rs._radius == 20);

    Sphere spheres6[] = {s1, s2, s3, s4};
    rs = CreateSphereFromSpheres(spheres6, 4);
    REQUIRE(rs._centre == Vec3f(5, 0, 0));
    REQUIRE(rs._radius == 25);
}
