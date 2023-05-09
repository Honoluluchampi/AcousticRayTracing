#include "primitives.hpp"

namespace ART {

// create rays which directions are radial
std::vector<Ray> create_rays(vec2d origin, int ray_count, int reflection_count)
{
  std::vector<Ray> rays(ray_count);
  for (int i = 0; i < ray_count; i++) {
    auto theta = M_PI * 2.f * static_cast<double>(i) / static_cast<double>(ray_count);
    vec2d direction = vec2d{std::cos(theta), std::sin(theta)};
    rays[i] = Ray{
      .direction = direction,
      .origin = origin,
      .ref_count = reflection_count,
      .length = 0.f
    };
  }
  return rays;
}

// returns 2D-point if intersects
// https://rootllama.wordpress.com/2014/06/20/ray-line-segment-intersection-test-in-2d/
double intersection_ray_line(const Ray &ray, const Line &line)
{
  const auto v1 = ray.origin - line.start;
  const auto v2 = line.end - line.start;
  const auto v3 = vec2d{-ray.direction.y(), ray.direction.x()};

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
double intersection_ray_sphere(const Ray &ray, const Sphere &sphere)
{
  double a = ray.direction.squaredNorm();
  double b = 2.f * (ray.direction.dot(ray.origin - sphere.center));
  double c = (ray.origin - sphere.center).squaredNorm() - sphere.radius * sphere.radius;

  auto judge = b * b - a * c;
  // return the closer intersection point
  if (judge > 0) {
    return (-2.f * b - std::sqrt(b * b - 4.f * a * c)) / (2.f * a);
  } else
    return NO_INTERSECTION;
}

} // namespace ART