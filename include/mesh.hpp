#pragma once

#include <cstddef>
#include <sys/types.h>
#include <vector>

namespace Eng {

class Material;

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

class Mesh {
public:
  Mesh() = default;
  Mesh(const std::vector<float>& rawData,
       const std::vector<unsigned int>& indices,
       const std::vector<VertexAttribute> &attributes, 
       size_t vertex_count, size_t stride);
  ~Mesh();

  Mesh(const Mesh&) = delete;
  Mesh& operator=(const Mesh&) = delete;

  Mesh(Mesh&& other) noexcept;
  Mesh& operator=(Mesh&& other) noexcept;

  void set_material(Material* material);
  Material* get_material() const;
  unsigned int get_VAO() const;
  size_t get_vertex_count() const;
  size_t get_index_count() const;

private:
  uint _VAO, _VBO, _EBO;
  size_t vertex_count;
  size_t index_count;
};

} // namespace Eng