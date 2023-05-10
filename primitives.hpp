#pragma once

#include <eigen3/Eigen/Dense>

using vec2d = Eigen::Vector2d;

// Acoustic Ray Tracing
namespace ART {

static double NO_INTERSECTION = std::numeric_limits<double>::min();
static uint32_t assign_id = 0;
using obj_id = uint32_t;

struct Sphere
{
  vec2d center;
  double radius;
  obj_id id = assign_id++;
};

struct Line
{
  vec2d start;
  vec2d end;
  obj_id id = assign_id++;
};

struct Room
{
  std::vector<Line> walls;
};

struct Ray
{
  vec2d direction;
  vec2d origin;
  int ref_count; // reflection count
  double acc_length = 0.f; // accumulative sum of the path length

  // return -1 if not intersected
  double intersection(const Line &line) const;
  double intersection(const Sphere &sphere) const;

  void reflect(const Line& wall, double length);
};

std::vector<Ray> create_rays(vec2d origin, int ray_count, int reflection_count);

} // namespace ART