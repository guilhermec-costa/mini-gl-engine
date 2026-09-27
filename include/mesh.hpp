#pragma once

#include "primitives.hpp"
#include <cstddef>
#include <sys/types.h>
#include <vector>

namespace Eng {

class Mesh {
public:
  Mesh(const std::vector<float>& rawData,
       const std::vector<VertexAttribute> &attributes, size_t vertex_count,
       size_t stride);

  Mesh(const Mesh&) = delete;
  Mesh& operator=(const Mesh&) = delete;
  Mesh(Mesh&& other) noexcept;
  ~Mesh();

  void draw() const;

private:
  uint _VAO, _VBO;
  size_t vertex_count;
};

} // namespace Eng