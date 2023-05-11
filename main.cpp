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
  ART::Scene scene_with_wall = {
    .walls {
      { .start = points[0], .end = points[1] },
      { .start = points[1], .end = points[2] },
      { .start = points[2], .end = points[3] },
      { .start = points[3], .end = points[0] },
      { .start = points[4], .end = points[5] }
    },
    .sources {
      { .center = { 0.5f, 0.5f }, .radius = 0.2f }
    }
  };

  vec2d listener1 = { 2.5f, 0.5f };
  vec2d listener2 = { 0.5f, 1.5f };

  auto ray_count = 500;
  auto rays1 = ART::create_rays(listener1, ray_count);
  auto rays2 = ART::create_rays(listener2, ray_count);

  ART::IR ir1 = {
    .sampling_rate = 200.f,
    .duration = 0.5f,
    .sound_speed = 310.f,
    .decrease_rate = 1.f
  };

  ART::IR ir2 = {
    .sampling_rate = 200.f,
    .duration = 0.5f,
    .sound_speed = 310.f,
    .decrease_rate = 1.f
  };

  ART::Scene small_room = {
    .walls {
      { .start = {0.f, 0.f}, .end = {1.f, 0.f} },
      { .start = {1.f, 1.f}, .end = {1.f, 0.f} },
      { .start = {1.f, 1.f}, .end = {0.f, 1.f} },
      { .start = {0.f, 1.f}, .end = {0.f, 0.f} }
    },
    .sources {
      { .center = {0.2f, 0.2f}, .radius = 0.1f }
    }
  };

  ART::IR ir3 = {
    .sampling_rate = 100.f,
    .duration = 1.f,
    .sound_speed = 310.f,
    .decrease_rate = 0.97f
  };

  vec2d listener3 = { 0.8f, 0.8f };
  auto rays3 = ART::create_rays(listener3, ray_count);

  auto max_ref_count = 200;
  ir1.compute_IR(rays1, scene_with_wall, max_ref_count);
  ir2.compute_IR(rays2, scene_with_wall, max_ref_count);
  ir3.compute_IR(rays3, small_room, max_ref_count);

  // write to the output file
  std::ofstream ofs1("/home/honolulu/programs/acoustic_ray_tracing/data/listener1.ir");
  for (auto& val : ir1.ir_series) {
    ofs1 << val / ray_count << std::endl;
  }

  std::ofstream ofs2("/home/honolulu/programs/acoustic_ray_tracing/data/listener2.ir");
  for (auto& val : ir2.ir_series) {
    ofs2 << val / ray_count << std::endl;
  }

  std::ofstream ofs3("/home/honolulu/programs/acoustic_ray_tracing/data/small_room.ir");
  for (auto& val : ir3.ir_series) {
    ofs3 << val / ray_count << std::endl;
  }
}
