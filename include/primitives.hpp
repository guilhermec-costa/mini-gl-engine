#pragma once

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