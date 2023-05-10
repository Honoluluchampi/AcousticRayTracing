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
  // calc amplitude
  double amplitude = std::pow(decrease_rate, ray.ref_count);

  // calc decay time
  double decay = ray.acc_length / sound_speed;
  if (decay > duration)
    return 0.f;

  // calc ir vector index
  auto index = static_cast<uint32_t>(decay * sampling_rate);

  ir_series[index] += amplitude;

  return amplitude;
}


} // namespace ART