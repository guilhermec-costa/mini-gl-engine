#include "primitives.hpp"
#include <glad/glad.h>
#include "glm/ext/matrix_transform.hpp"
#include "material.hpp"
#include "mesh.hpp"

namespace Eng {

glm::mat4 Transform::matrix() const {
  glm::mat4 mat = glm::mat4(1.0);
  mat = glm::translate(mat, position);
  mat = glm::rotate(mat, glm::radians(rotation_angle), rotation_axis);
  mat = glm::scale(mat, scale_factor);

  return mat;
}

void Transform::set_position(const glm::vec3& pos) {
  position = pos;
}

void Transform::move(const glm::vec3& offset) {
  position += offset;
};

void Transform::rotate(float angle, const glm::vec3& axis) {
  rotation_angle += angle;
  rotation_axis = glm::normalize(axis);
}

void Transform::set_rotate(float angle, const glm::vec3& axis) {
  rotation_angle = angle;
  rotation_axis = glm::normalize(axis);
}

void Transform::set_scale(const glm::vec3& factor) {
  scale_factor = factor;
}

void Transform::scale(const glm::vec3& factor) {
  scale_factor *= factor;
}
}

Eng::Mesh make_quad(glm::vec3 v1, glm::vec3 v2,glm::vec3 v3, glm::vec3 v4) {
  return Eng::Mesh(
    std::vector{
      v1.x, v1.y, v1.z, 0.0f, 1.0f,
      v2.x, v2.y, v2.z, 1.0f, 1.0f,
      v3.x, v3.y, v3.z, 1.0f, 0.0f,
      v4.x, v4.y, v4.z, 0.0f, 0.0f,
    },
    {0, 1, 3, 1, 2, 3},
    std::vector{
      Eng::VertexAttribute(0, 3, 0),
      Eng::VertexAttribute(1, 2, sizeof(float) * 3)
    },
    4,
    sizeof(float) * 5); 
}

Eng::Mesh make_triangle(glm::vec3 v1, glm::vec3 v2,glm::vec3 v3) {
  return Eng::Mesh(
    std::vector{
      v1.x, v1.y, v1.z, 0.5f, 1.0f,
      v2.x, v2.y, v2.z, 1.0f, 0.0f,
      v3.x, v3.y, v3.z, 0.0f, 0.0f,
    },
    {},
    std::vector{
      Eng::VertexAttribute(0, 3, 0),
      Eng::VertexAttribute(1, 2, sizeof(float) * 3)
    },
    3,
    sizeof(float) * 5
  );
}

Eng::Mesh make_cube(const std::vector<float>& data) {
  return Eng::Mesh(
    data,
    {},
    std::vector{
      Eng::VertexAttribute(0, 3, 0),
      Eng::VertexAttribute(1, 2, sizeof(float) * 3)
    },
    data.size() / 5,
    sizeof(float) * 5);
}

Eng::Mesh make_cube_with_normals(const std::vector<float>& data) {
  return Eng::Mesh(
    data,
    {},
    std::vector{
      Eng::VertexAttribute{0, 3, 0},
      Eng::VertexAttribute{1, 2, sizeof(float) * 3},
      Eng::VertexAttribute{2, 3, sizeof(float) * 5},
    },
    data.size() / 8,
    sizeof(float) * 8
  );
}