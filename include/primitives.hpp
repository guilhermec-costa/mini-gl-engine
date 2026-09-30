#pragma once

#include "glm/ext/vector_float3.hpp"
#include <cstddef>

namespace Eng {

class VertexAttribute {
public:
  VertexAttribute() = delete;
  VertexAttribute(unsigned int location, unsigned int components, size_t offset)
      : location(location), components(components), offset(offset) {};

public:
  unsigned int location;
  unsigned int components;
  size_t offset;
};
} // namespace Eng

void draw_quad(unsigned int width, unsigned int height, glm::vec3 origin);