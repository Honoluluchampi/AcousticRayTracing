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
      { points[0], points[1] },
      { points[1], points[2] },
      { points[2], points[3] },
      { points[3], points[0] },
      { points[4], points[5] }
  }};

  ART::Sphere source = {
    .center = { 0.5f, 0.5f },
    .radius = 0.4f
  };

  vec2d listener1 = { 2.5f, 0.5f };
  vec2d listener2 = { 0.5f, 1.5f };


}
