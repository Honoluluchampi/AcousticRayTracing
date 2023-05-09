#pragma once

#include <eigen3/Eigen/Dense>

using vec2d = Eigen::Vector2d;

// Acoustic Ray Tracing
namespace ART {

static double NO_INTERSECTION = -1;

struct Ray
{
  vec2d direction;
  vec2d origin;
  int ref_count; // reflection count
  double length;
};

struct Sphere
{
  vec2d center;
  double radius;
};

struct Line
{
  vec2d start;
  vec2d end;
};

struct Room
{
  std::vector<Line> walls;
};

std::vector<Ray> create_rays(vec2d origin, int ray_count, int reflection_count);

// return -1 if not intersected
double intersection_ray_line(const Ray &ray, const Line &line);
double intersection_ray_sphere(const Ray &ray, const Sphere &sphere);

} // namespace ART