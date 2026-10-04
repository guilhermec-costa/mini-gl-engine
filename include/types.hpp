#pragma once

#include "glm/common.hpp"

namespace EngTypes {

constexpr float MAX_COLOR = 255.0f;
constexpr float MIN_COLOR = 0.0f;

struct Color {
  // normalized red value
  float r = 0.0;
  // normalized green value
  float g = 0.0;
  // normalized blue value
  float b = 0.0;
  // normalized alpha value
  float a = 1.0;

  Color() = default;
  Color(float r, float g, float b, float a, bool norm=false)
      : r(norm ? normalize(r) : r), g(norm ? normalize(g) : g),
        b(norm ? normalize(b) : b), a(norm ? normalize(a) : a) {}

private:
  float normalize(float v) const {
    const float clamped_color = glm::clamp<float>(v, MIN_COLOR, MAX_COLOR);
    return clamped_color / MAX_COLOR;
  }
};

} // namespace EngTypes