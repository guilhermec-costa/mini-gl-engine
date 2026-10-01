#include <glad/glad.h>
#include "mesh.hpp"

namespace Eng {

Mesh::Mesh(const std::vector<float> &raw_data,
           const std::vector<unsigned int>& indices,
           const std::vector<VertexAttribute> &attributes, size_t _vertex_count,
           size_t stride, Material *_material)
  : index_count(0), vertex_count(0) {
  uint VBO, VAO, EBO;
  glGenBuffers(1, &VBO);

  if (!indices.empty()) {
    index_count = indices.size();
    glGenBuffers(1, &EBO);
  }
  glGenVertexArrays(1, &VAO);

  _VBO = VBO;
  _VAO = VAO;

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * raw_data.size(),
               raw_data.data(), GL_STATIC_DRAW);

  for (const VertexAttribute &attr : attributes) {
    glVertexAttribPointer(attr.location, attr.components, GL_FLOAT, GL_FALSE,
                          stride, (void *)attr.offset);
    glEnableVertexAttribArray(attr.location);
  }
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  if (index_count > 0) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);
  }
  glBindVertexArray(0);

  vertex_count = _vertex_count;
  material = _material;
};

Mesh::Mesh(Mesh &&other) noexcept
    : _VAO(other._VAO), _VBO(other._VBO), vertex_count(other.vertex_count) {
  other._VAO = 0;
  other._VBO = 0;
}

Mesh &Mesh::operator=(Mesh &&other) noexcept {
  if (this != &other) {
    glDeleteBuffers(1, &_VBO);
    glDeleteVertexArrays(1, &_VAO);

    _VAO = other._VAO;
    _VBO = other._VBO;
    other._VAO = 0;
    other._VBO = 0;
  }

  return *this;
}

Mesh::~Mesh() {
  glDeleteBuffers(1, &_VBO);
  glDeleteVertexArrays(1, &_VAO);
}

Material *Mesh::get_material() const { return material; }

size_t Mesh::get_vertex_count() const { return vertex_count; }
size_t Mesh::get_index_count() const { return index_count; }

unsigned int Mesh::get_VAO() const { return _VAO; }

void Mesh::set_material(Material *m) { material = m; }

} // namespace Eng