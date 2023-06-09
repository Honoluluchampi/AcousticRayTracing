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


TEST(assign_id, line_sphere) {
  ART::Line line1;
  ART::Line line2;
  ART::Sphere sphere1;
  ART::Line line3;
  ART::Sphere sphere2;

  EXPECT_EQ(line1.id,   0);
  EXPECT_EQ(line2.id,   1);
  EXPECT_EQ(sphere1.id, 2);
  EXPECT_EQ(line3.id,   3);
  EXPECT_EQ(sphere2.id, 4);
}

TEST(util_functions, create_rays) {
  auto rays = ART::create_rays({2.f, 3.f}, 8);

  // common parameters
  EXPECT_EQ(rays.size(), 8);
  EXPECT_EQ(rays[0].origin, vec2d(2.f, 3.f));
  EXPECT_EQ(rays[0].ref_count, 0);
  EXPECT_EQ(rays[0].acc_length, 0.f);

  // direction
  NEARLY_EQ(rays[0].direction, vec2d(1.f, 0.f));
  NEARLY_EQ(rays[2].direction, vec2d(0.f, 1.f));
  NEARLY_EQ(rays[3].direction, vec2d(-std::sqrt(2.f) / 2.f, std::sqrt(2.f) / 2.f));
  NEARLY_EQ(rays[7].direction, vec2d(std::sqrt(2.f) / 2.f, -std::sqrt(2.f) / 2.f));
}

TEST(intersection, ray_line_x) {
  ART::Ray ray = {.direction = {1.f, 0.f}, .origin = {3.f, 2.f}};
  ART::Line line1 = {.start = {10.f, 1.f}, .end = {10.f, 3.f}};
  ART::Line line2 = {.start = {-10.f, 1.f}, .end = {-10.f, 3.f}};

  EXPECT_EQ(ray.intersection(line1), 7.f);
  EXPECT_EQ(ray.intersection(line2), ART::NO_INTERSECTION);
}

TEST(intersection, ray_line_y) {
  ART::Ray ray = {.direction = {0.f, 1.f}, .origin = {2.f, 3.f}};
  ART::Line line1 = {.start = {1.f, 10.f}, .end = {3.f, 10.f}};
  ART::Line line2 = {.start = {1.f, -10.f}, .end = {3.f, -10.f}};

  EXPECT_EQ(ray.intersection(line1), 7.f);
  EXPECT_EQ(ray.intersection(line2), ART::NO_INTERSECTION);
}

TEST(intersection, ray_line) {
  ART::Ray ray = {.direction = {-1.f, -2.f}, .origin = {-2.f, -1.f}};
  // you have to normalize initial direction
  ray.direction.normalize();
  ART::Line line1 = {.start = {-5.f, -3.f}, .end = {-1.f, -3.f}};
  ART::Line line2 = {.start = {-5.f, -3.f}, .end = {-2.f, -1.f}};

  NEARLY_EQ(ray.intersection(line1), std::sqrt(5.f));
  NEARLY_EQ(ray.intersection(line2), ART::NO_INTERSECTION);
}

TEST(intersection, ray_sphere) {
  ART::Ray ray1 = {.direction = {1.f, 0.f}, .origin = {0.f, 4.f}};
  ART::Sphere sphere1 = {.center = {10.f, 4.f}, .radius = 3.f};
  ART::Sphere sphere2 = {.center = {10.f, 7.f}, .radius = 3.f};

  EXPECT_EQ(ray1.intersection(sphere1), 7.f);
  EXPECT_EQ(ray1.intersection(sphere2), 10.f);

  ART::Ray ray2 = {.direction = {-1.f, 0.f}, .origin = {3.f, -3.f + std::sqrt(2.f)}};
  ART::Sphere sphere3 = {.center = {-2.f, -3.f}, .radius = 2.f };
  ART::Sphere sphere4 = {.center = {5.f, -3.f}, .radius = 2.f };
  ART::Sphere sphere5 = {.center = {-2.f, -5.f}, .radius = 2.f };

  NEARLY_EQ(ray2.intersection(sphere3), 5.f - std::sqrt(2.f));
  EXPECT_EQ(ray2.intersection(sphere4), NO_INTERSECTION);
  EXPECT_EQ(ray2.intersection(sphere5), NO_INTERSECTION);
}

TEST(reflect, ray_line) {
  ART::Ray ray = {.direction = {1.f, 0.f}, .origin = {0.f, 0.f}};
  ART::Line wall1 = {.start = {10.f, 1.f}, .end = {10.f, -1.f}};
  ART::Line wall2 = {.start = {-10.f, 1.f}, .end = {-10.f, -1.f}};

  auto path_length1 = ray.intersection(wall1);
  ray.reflect(wall1, path_length1);

  NEARLY_EQ(ray.acc_length, 10.f);
  EXPECT_EQ(ray.ref_count, 1);
  NEARLY_EQ(ray.direction, vec2d(-1.f, 0.f));
  NEARLY_EQ(ray.origin, vec2d(10.f, 0.f));

  auto path_length2 = ray.intersection(wall2);
  ray.reflect(wall2, path_length2);

  NEARLY_EQ(ray.acc_length, 30.f);
  EXPECT_EQ(ray.ref_count, 2);
  NEARLY_EQ(ray.direction, vec2d(1.f, 0.f));
  NEARLY_EQ(ray.origin, vec2d(-10.f, 0.f));
}

TEST(closest_hit, ray_line) {
  ART::Ray ray = {.direction = {1.f, 0.f}, .origin = {0.f, 0.f}};
  std::vector<ART::Line> walls = {
    {.start = {5.f, 1.f}, .end = {5.f, -1.f}},
    {.start = {-5.f, 1.f}, .end = {-5.f, -1.f}},
    {.start = {3.f, 1.f}, .end = {3.f, -1.f}},
    {.start = {10.f, 1.f}, .end = {10.f, -1.f}}
  };

  auto ans = ray.closest_hit(walls);
  EXPECT_EQ(ans.second, 3.f);
  EXPECT_EQ(ans.first.id, walls[2].id);
}

TEST(closest_hit, ray_sphere)
{
  ART::Ray ray = {.direction = {1.f, 0.f}, .origin = {0.f, 0.f}};
  std::vector<ART::Sphere> spheres = {
    {.center = {5.f, 0.f}, .radius = 1.f},
    {.center = {-5.f, 0.f}, .radius = 3.f},
    {.center = {3.f, 0.f}, .radius = 1.f},
    {.center = {10.f, 0.f}, .radius = 3.f}
  };

  auto ans = ray.closest_hit(spheres);
  EXPECT_EQ(ans.second, 2.f);
  EXPECT_EQ(ans.first.id, spheres[2].id);
}

TEST(ray_trace, trace) {
  ART::Ray ray = {.direction = {1.f, 0.f}, .origin = {0.f, 0.f}};
  std::vector<ART::Sphere> sources = {
    {.center = {4.f, 4.f}, .radius = 1.f}
  };
  std::vector<ART::Line> walls = {
    {.start = {3.f, -1.f}, .end = {5.f, 1.f}},
    {.start = {1.f, 1.f},  .end = {2.f, 2.f}}
  };

  EXPECT_TRUE(ray.trace(walls, sources, 1));
  EXPECT_EQ(ray.source_hit_infos[0].first, 1);
  EXPECT_EQ(ray.source_hit_infos[0].second, 7.f);
}

// impulse response -------------------------------------------------------------------------
TEST(impulse_response, ctor)
{
  ART::IR ir = {.sampling_rate = 1000.f, .duration = 2.f, .sound_speed = 1.f, .decrease_rate = 0.8f};
  EXPECT_EQ(ir.sampling_rate, 1000.f);
  EXPECT_EQ(ir.duration, 2.f);
  EXPECT_EQ(ir.sound_speed, 1.f);
  EXPECT_EQ(ir.decrease_rate, 0.8f);
}

TEST(impulse_response, add_ray_hit)
{
  ART::IR ir = {.sampling_rate = 100.f, .duration = 2.f, .sound_speed = 10.f, .decrease_rate = 0.8f};
  ir.ir_series.resize(200, 0.f);
  ART::Ray ray1 {.direction = {1.f, 0.f}, .origin = {0.f, 0.f}, .ref_count = 2, .acc_length = 10.f,
                 .source_hit_infos = {{1, 10.f}, {2, 10.f}, {3, 15.f}}};
  ART::Ray ray2 {.direction = {1.f, 0.f}, .origin = {0.f, 0.f}, .ref_count = 2, .acc_length = 30.f};

  auto amp1 = ir.add_ray_hit(ray1);
  NEARLY_EQ(amp1, 0.8f + 0.64f + 0.512f);
  NEARLY_EQ(ir.ir_series[100], 1.44f);
  NEARLY_EQ(ir.ir_series[150], 0.512f);

  auto amp2 = ir.add_ray_hit(ray2);
  NEARLY_EQ(amp2, 0.f);
}
} // namespace ART