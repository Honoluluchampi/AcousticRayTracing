#include "primitives.hpp"
#include <iostream>
#include <fstream>

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

  auto ray_count = 500;
  auto rays1 = ART::create_rays(listener1, ray_count);
  auto rays2 = ART::create_rays(listener2, ray_count);

  ART::IR ir1 = {
    .sampling_rate = 100.f,
    .duration = 1.f,
    .sound_speed = 310.f,
    .decrease_rate = 0.95f
  };

  ART::IR ir2 = {
    .sampling_rate = 100.f,
    .duration = 1.f,
    .sound_speed = 310.f,
    .decrease_rate = 0.95f
  };

  ir1.compute_IR(rays1, room.walls, {source}, 200);
  ir2.compute_IR(rays2, room.walls, {source}, 200);

  // write to the output file
  std::ofstream ofs1("/home/honolulu/programs/acoustic_ray_tracing/data/listener1.ir");
  for (auto& val : ir1.ir_series) {
    ofs1 << val / ray_count << std::endl;
  }

  std::ofstream ofs2("/home/honolulu/programs/acoustic_ray_tracing/data/listener2.ir");
  for (auto& val : ir2.ir_series) {
    ofs2 << val / ray_count << std::endl;
  }
}
