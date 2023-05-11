#pragma once

#include <eigen3/Eigen/Dense>

using vec2d = Eigen::Vector2d;

// Acoustic Ray Tracing
namespace ART {

static double NO_INTERSECTION = std::numeric_limits<double>::max();
using  obj_id = uint32_t;
static obj_id assign_id = 0;

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

struct Scene
{
  std::vector<Line> walls;
  std::vector<Sphere> sources;
};

struct Ray
{
  vec2d direction;
  vec2d origin;
  int ref_count = 0; // reflection count
  double acc_length = 0.f; // accumulative sum of the path length

  // ref_count and acc_length of each source hit
  std::vector<std::pair<int, double>> source_hit_infos;

  // return -1 if not intersected
  double intersection(const Line &line) const;
  double intersection(const Sphere &sphere) const;

  // returns closest hit object and the length from ray.origin
  template <typename Obj>
  std::pair<const Obj&, double> closest_hit(const std::vector<Obj>& objs) const;

  void reflect(const Line& wall, double length);
  void hit_sphere(const Sphere& sphere, double length);
  // returns true if the ray intersects with any sound sources
  bool trace(const std::vector<Line>& walls, const std::vector<Sphere>& sources, int max_ref_count);
};

// Impulse Response
struct IR
{
  // returns added ray's amplitude
  double add_ray_hit(const Ray& ray);

  void compute_IR(
    std::vector<Ray>& rays,
    const Scene& scene,
    int max_ref_count);

  double sampling_rate;
  double duration;
  double sound_speed; // could be temporary
  double decrease_rate = 0.8f;
  std::vector<double> ir_series;
};

std::vector<Ray> create_rays(vec2d origin, int ray_count);

// impl
template <typename Obj>
std::pair<const Obj&, double> Ray::closest_hit(const std::vector<Obj>& objs) const
{
  auto closest_length = NO_INTERSECTION;
  // this will be invalid if closest_length is NO_INTERSECTION at the last of the function
  int closest_id = 0;

  for (int i = 0; i < objs.size(); i++) {
    auto new_length = intersection(objs[i]);
    if (new_length < closest_length) {
      closest_length = new_length;
      closest_id = i;
    }
  }

  return {objs[closest_id], closest_length};
}

} // namespace ART