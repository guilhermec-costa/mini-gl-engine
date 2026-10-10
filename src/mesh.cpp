#include <vendor/glad/glad.h>
#include "mesh.hpp"

namespace Eng {

Mesh::Mesh(const std::vector<float> &raw_data,
           const std::vector<unsigned int>& indices,
           const std::vector<VertexAttribute> &attributes, 
           size_t vertex_count, size_t stride)
  : index_count(indices.size()), vertex_count(vertex_count) {
  glGenVertexArrays(1, &_VAO);
  glGenBuffers(1, &_VBO);

  glBindVertexArray(_VAO);

  glBindBuffer(GL_ARRAY_BUFFER, _VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * raw_data.size(),
               raw_data.data(), GL_STATIC_DRAW);

  for (const VertexAttribute &attr : attributes) {
    glVertexAttribPointer(attr.location, attr.components, GL_FLOAT, GL_FALSE,
                          stride, (void *)attr.offset);
    glEnableVertexAttribArray(attr.location);
  }
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  if(!indices.empty()) {
    glGenBuffers(1, &_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(
      GL_ELEMENT_ARRAY_BUFFER, 
      sizeof(unsigned int) * indices.size(), 
      indices.data(), 
      GL_STATIC_DRAW
    );
  }
  glBindVertexArray(0);
};

Mesh::Mesh(Mesh &&other) noexcept
    : _VAO(other._VAO), _VBO(other._VBO), _EBO(other._EBO),
      vertex_count(other.vertex_count), index_count(other.index_count) {
  other._VAO = 0;
  other._VBO = 0;
  other._EBO = 0;
}

Mesh &Mesh::operator=(Mesh &&other) noexcept {
  if (this != &other) {
    glDeleteBuffers(1, &_VBO);
    glDeleteBuffers(1, &_EBO);
    glDeleteVertexArrays(1, &_VAO);

    _VAO = other._VAO;
    _VBO = other._VBO;
    _EBO = other._EBO;
    vertex_count = other.vertex_count;
    index_count = other.index_count;
    other._VAO = 0;
    other._VBO = 0;
    other._EBO = 0;
    other.vertex_count = 0;
    other.index_count = 0;
  }

  return *this;
}

Mesh::~Mesh() {
  glDeleteBuffers(1, &_VBO);
  glDeleteBuffers(1, &_EBO);
  glDeleteVertexArrays(1, &_VAO);
}

size_t Mesh::get_vertex_count() const { return vertex_count; }
size_t Mesh::get_index_count() const { return index_count; }

unsigned int Mesh::get_VAO() const { return _VAO; }

} // namespace Eng