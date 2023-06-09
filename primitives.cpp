#include "primitives.hpp"

namespace ART {

void Ray::reflect(const Line &wall, double length)
{
  ref_count += 1;
  acc_length += length;

  // calc new origin (reflecting point)
  origin = origin + direction * length;

  // calc new direction
  auto wall_d = (wall.end - wall.start);
  auto wall_n = vec2d{-wall_d.y(), wall_d.x()}.normalized();
  direction = direction - (2.f * direction.dot(wall_n)) * wall_n;
  direction.normalize();
}

void Ray::hit_sphere(const Sphere &sphere, double length)
{ acc_length += length; }

bool Ray::trace(const std::vector<Line>& walls, const std::vector<Sphere>& sources, int max_ref_count)
{
  bool hit_source = false;

  while (ref_count <= max_ref_count) {
    const auto c_wall   = closest_hit(walls);
    const auto c_source = closest_hit(sources);

    // ray is getting out of the domain
    if (c_wall.second == NO_INTERSECTION && c_source.second == NO_INTERSECTION)
      return hit_source;

    // if intersects with source
    if (c_source.second <= c_wall.second) {
      source_hit_infos.emplace_back(ref_count, acc_length + c_source.second);
      hit_source = true;
    }

    // intersects with wall
    if (c_wall.second == NO_INTERSECTION) {
      return hit_source;
    }
    reflect(c_wall.first, c_wall.second);
  }

  return hit_source;
}

// returns 2D-point if intersects
// https://rootllama.wordpress.com/2014/06/20/ray-line-segment-intersection-test-in-2d/
double Ray::intersection(const Line &line) const
{
  const auto v1 = origin - line.start;
  const auto v2 = line.end - line.start;
  const auto v3 = vec2d{-direction.y(), direction.x()};

  double eps = 1e-6;
  if (std::abs(v2.dot(v3)) < eps)
    return NO_INTERSECTION;

  double t1 = (v2.x() * v1.y() - v2.y() * v1.x()) / (v2.dot(v3));
  double t2 = (v1.dot(v3)) / (v2.dot(v3));

  if (t1 > 0 && 0 <= t2 && t2 <= 1) {
    return t1;
  }

  return NO_INTERSECTION;
}

// assume source is represented as sphere
// TODO : consider rays from inside the sphere
double Ray::intersection(const Sphere &sphere) const
{
  double a = direction.squaredNorm();
  double b = 2.f * (direction.dot(origin - sphere.center));
  double c = (origin - sphere.center).squaredNorm() - sphere.radius * sphere.radius;

  auto judge = b * b - 4.f * a * c;
  // return the closer intersection point
  if (judge >= 0) {
    auto t = (-b - std::sqrt(b * b - 4.f * a * c)) / (2.f * a);
    if (t > 0.f)
      return t;
  }

  return NO_INTERSECTION;
}

// create rays which directions are radial
std::vector<Ray> create_rays(vec2d origin, int ray_count)
{
  std::vector<Ray> rays(ray_count);
  for (int i = 0; i < ray_count; i++) {
    auto theta = M_PI * 2.f * static_cast<double>(i) / static_cast<double>(ray_count);
    vec2d direction = vec2d{std::cos(theta), std::sin(theta)};
    rays[i] = Ray{
      .direction = direction.normalized(),
      .origin = origin
    };
  }
  return rays;
}

// Impulse Response ---------------------------------------------------------------
double IR::add_ray_hit(const Ray &ray)
{
  double whole_amplitude = 0.f;

  // calc amplitude
  for (const auto& hit_info : ray.source_hit_infos) {
    double amplitude = std::pow(decrease_rate, hit_info.first);

    // calc decay time
    double decay = hit_info.second / sound_speed;
    if (decay > duration)
      return 0.f;

    // calc ir vector index
    auto index = static_cast<uint32_t>(decay * sampling_rate);

    ir_series[index] += amplitude;
    whole_amplitude += amplitude;
  }

  return whole_amplitude;
}

void IR::compute_IR(
  std::vector<Ray> &rays,
  const Scene& scene,
  int max_ref_count)
{
  ir_series.resize(static_cast<size_t>(sampling_rate * duration), 0.f);

  // trace each rays
  for (auto& ray : rays) {
    if (ray.trace(scene.walls, scene.sources, max_ref_count)) {
      add_ray_hit(ray);
    }
  }
}
} // namespace ART