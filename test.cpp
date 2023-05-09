#include "primitives.hpp"
#include <gtest/gtest.h>

// nearly equal
bool NEQ(const vec2d& a, const vec2d& b, double eps = 1e-6) {
  return std::abs(a.x() - b.x()) < eps && std::abs(a.y() - b.y()) < eps;
}

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
  ART::Ray ray = { .direction = { 1.f, 0.f }, .origin = { 3.f, 2.f } };
  ART::Line line1 = { .start = { 10.f, 1.f }, .end = { 10.f, 3.f } };
  ART::Line line2 = { .start = { -10.f, 1.f }, .end = { -10.f, 3.f } };

  EXPECT_EQ(ART::intersection_ray_line(ray, line1), 7.f);
  EXPECT_EQ(ART::intersection_ray_line(ray, line2), ART::NO_INTERSECTION);
}

TEST(intersection, ray_line_y) {
  ART::Ray ray = { .direction = { 0.f, 1.f }, .origin = { 2.f, 3.f } };
  ART::Line line1 = { .start = { 1.f, 10.f }, .end = { 3.f, 10.f } };
  ART::Line line2 = { .start = { 1.f, -10.f }, .end = { 3.f, -10.f } };

  EXPECT_EQ(ART::intersection_ray_line(ray, line1), 7.f);
  EXPECT_EQ(ART::intersection_ray_line(ray, line2), ART::NO_INTERSECTION);
}

TEST(intersection, ray_sphere) {

}