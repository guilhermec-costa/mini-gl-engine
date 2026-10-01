#pragma once

namespace EngTypes {

struct Color {
  float r = 0.0;
  float g = 0.0;
  float b = 0.0;
  float a = 1.0;

  Color() = default;
  Color(float r, float g, float b, float a)
    : r(r), g(g), b(b), a(a) {}
};

}