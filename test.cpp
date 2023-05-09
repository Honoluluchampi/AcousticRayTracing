#include "primitives.hpp"
#include <gtest/gtest.h>

namespace ART {

static double eps = 1e-6;

// nearly equal
bool NEQ(const vec2d &a, const vec2d &b) {
  return std::abs(a.x() - b.x()) < eps && std::abs(a.y() - b.y()) < eps;
}

bool NEQ(double a, double b) {
  return std::abs(a - b) < eps;
}

#define NEARLY_EQ(a, b) EXPECT_TRUE(NEQ(a, b))

TEST(util_functions, create_rays) {
  auto rays = ART::create_rays({2.f, 3.f}, 8, 100);

  // common parameters
  EXPECT_EQ(rays.size(), 8);
  EXPECT_EQ(rays[0].origin, vec2d(2.f, 3.f));
  EXPECT_EQ(rays[0].ref_count, 100);
  EXPECT_EQ(rays[0].length, 0.f);

  // direction
  EXPECT_TRUE(NEQ(rays[0].direction, vec2d(1.f, 0.f)));
  EXPECT_TRUE(NEQ(rays[2].direction, vec2d(0.f, 1.f)));
  EXPECT_TRUE(NEQ(rays[3].direction, vec2d(-std::sqrt(2.f) / 2.f, std::sqrt(2.f) / 2.f)));
  EXPECT_TRUE(NEQ(rays[7].direction, vec2d(std::sqrt(2.f) / 2.f, -std::sqrt(2.f) / 2.f)));
}

TEST(intersection, ray_line_x) {
  ART::Ray ray = {.direction = {1.f, 0.f}, .origin = {3.f, 2.f}};
  ART::Line line1 = {.start = {10.f, 1.f}, .end = {10.f, 3.f}};
  ART::Line line2 = {.start = {-10.f, 1.f}, .end = {-10.f, 3.f}};

  EXPECT_EQ(ART::intersection_ray_line(ray, line1), 7.f);
  EXPECT_EQ(ART::intersection_ray_line(ray, line2), ART::NO_INTERSECTION);
}

TEST(intersection, ray_line_y) {
  ART::Ray ray = {.direction = {0.f, 1.f}, .origin = {2.f, 3.f}};
  ART::Line line1 = {.start = {1.f, 10.f}, .end = {3.f, 10.f}};
  ART::Line line2 = {.start = {1.f, -10.f}, .end = {3.f, -10.f}};

  EXPECT_EQ(ART::intersection_ray_line(ray, line1), 7.f);
  EXPECT_EQ(ART::intersection_ray_line(ray, line2), ART::NO_INTERSECTION);
}

TEST(intersection, ray_line) {
  ART::Ray ray = {.direction = {-1.f, -2.f}, .origin = {-2.f, -1.f}};
  // you have to normalize initial direction
  ray.direction.normalize();
  ART::Line line1 = {.start = {-5.f, -3.f}, .end = {-1.f, -3.f}};
  ART::Line line2 = {.start = {-5.f, -3.f}, .end = {-2.f, -1.f}};

  NEARLY_EQ(ART::intersection_ray_line(ray, line1), std::sqrt(5.f));
  NEARLY_EQ(ART::intersection_ray_line(ray, line2), ART::NO_INTERSECTION);
}

TEST(intersection, ray_sphere) {
  ART::Ray ray1 = {.direction = {1.f, 0.f}, .origin = {0.f, 4.f}};
  ART::Sphere sphere1 = {.center = {10.f, 4.f}, .radius = 3.f};
  ART::Sphere sphere2 = {.center = {10.f, 7.f}, .radius = 3.f};

  EXPECT_EQ(ART::intersection_ray_sphere(ray1, sphere1), 7.f);
  EXPECT_EQ(ART::intersection_ray_sphere(ray1, sphere2), 10.f);

  ART::Ray ray2 = {.direction = {-1.f, 0.f}, .origin = {3.f, -3.f + std::sqrt(2.f)}};
  ART::Sphere sphere3 = {.center = {-2.f, -3.f}, .radius = 2.f };
  ART::Sphere sphere4 = {.center = {5.f, -3.f}, .radius = 2.f };
  ART::Sphere sphere5 = {.center = {-2.f, -5.f}, .radius = 2.f };

  NEARLY_EQ(ART::intersection_ray_sphere(ray2, sphere3), 5.f - std::sqrt(2.f));
  EXPECT_EQ(ART::intersection_ray_sphere(ray2, sphere4), NO_INTERSECTION);
  EXPECT_EQ(ART::intersection_ray_sphere(ray2, sphere5), NO_INTERSECTION);
}

} // namespace ART