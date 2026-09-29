#pragma once

#include <cstddef>

namespace Eng {

struct VertexAttribute {
  unsigned int location;
  unsigned int components;
  size_t offset;
};
} // namespace Eng