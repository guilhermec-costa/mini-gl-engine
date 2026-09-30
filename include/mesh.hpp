#pragma once

#include "material.hpp"
#include "primitives.hpp"
#include <cstddef>
#include <sys/types.h>
#include <vector>

namespace Eng {

class Mesh {
public:
  Mesh(const std::vector<float>& rawData,
       const std::vector<VertexAttribute> &attributes, size_t vertex_count,
       size_t stride, Material* material);
  ~Mesh();

  Mesh(const Mesh&) = delete;
  Mesh& operator=(const Mesh&) = delete;

  Mesh(Mesh&& other) noexcept;
  Mesh& operator=(Mesh&& other) noexcept;

  void set_material(Material* material);
  Material* get_material() const;
  unsigned int get_VAO() const;
  int get_vertex_count() const;

public:
  Material* material = NULL;

private:
  uint _VAO, _VBO;
  size_t vertex_count;
};

} // namespace Eng