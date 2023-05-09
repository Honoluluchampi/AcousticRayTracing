#include "primitives.hpp"
#include <gtest/gtest.h>

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