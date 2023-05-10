#include "primitives.hpp"
#include <iostream>

const std::array<vec2d, 6> points = {
  vec2d{ 0.f, 0.f },
  vec2d{ 3.f, 0.f },
  vec2d{ 3.f, 3.f },
  vec2d{ 0.f, 3.f },
  vec2d{ 0.f, 1.f },
  vec2d{ 2.f, 1.f },
};

int main()
{
  ART::Room room = {
    .walls {
      { .start = points[0], .end = points[1] },
      { .start = points[1], .end = points[2] },
      { .start = points[2], .end = points[3] },
      { .start = points[3], .end = points[0] },
      { .start = points[4], .end = points[5] }
  }};

  ART::Sphere source = {
    .center = { 0.5f, 0.5f },
    .radius = 0.4f
  };

  vec2d listener1 = { 2.5f, 0.5f };
  vec2d listener2 = { 0.5f, 1.5f };

  auto rays = ART::create_rays(listener2, 500);

  ART::IR ir = {
    .sampling_rate = 100.f,
    .duration = 1.f,
    .sound_speed = 310.f,
    .decrease_rate = 0.95f
  };

  ir.compute_IR(rays, room.walls, {source}, 200);

  for (auto& val : ir.ir_series) {
    std::cout << val << std::endl;
  }
}
