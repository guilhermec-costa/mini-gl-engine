#include "primitives.hpp"
#include <vendor/glad/glad.h>
#include <numbers>
#include <ostream>
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float2.hpp"
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

Eng::Mesh make_sphere(float radius, int stack_count, int sector_count) {
  const double PI = std::numbers::pi;
  const float SECTOR_STEP = 2 * PI / sector_count;
  const float STACK_STEP = PI / stack_count;

  glm::vec3 pos;
  glm::vec2 uv;
  glm::vec3 normal;

  float stack_radius;
  float stack_angle, sector_angle;
  float length_inverse = 1.0f / radius;
  // normalized_vec = = vec/length = vec * length_inverse

  std::vector<float> positions;
  std::vector<float> uvs;
  std::vector<float> normals;

  for(size_t st = 0; st<= stack_count; ++st) {
    stack_angle = PI / 2 - st * STACK_STEP; //  from PI/2 to -PI/2
    stack_radius = radius * cosf(stack_angle);
    pos.z = radius * sinf(stack_angle); // considering z as the vertical axis

    for(size_t se = 0; se<= sector_count; ++se) {
      sector_angle = se * SECTOR_STEP; // from 0 to 2PI
      pos.x = stack_radius * cosf(sector_angle);
      pos.y = stack_radius * sinf(sector_angle);
      positions.push_back(pos.x);
      positions.push_back(pos.y);
      positions.push_back(pos.z);

      normal.x = pos.x * length_inverse;
      normal.y = pos.y * length_inverse;
      normal.z = pos.z * length_inverse;
      normals.push_back(normal.x);
      normals.push_back(normal.y);
      normals.push_back(normal.z);

      uv.s = static_cast<float>(se) / sector_count;
      uv.t = static_cast<float>(st) / stack_count;
      uvs.push_back(uv.s);
      uvs.push_back(uv.t);
    }
  }

  std::vector<unsigned int> indices;
  int k1, k2;

  for(size_t st=0; st < stack_count; ++st) {
    k1 = st * (sector_count + 1);
    k2 = k1 + sector_count + 1;

    for(size_t se = 0; se < sector_count; ++se, ++k1, ++k2) {
      // 2 triangles per sector excluding first and last stacks
      if(st != 0) {
        // k1 => k2 => k1+1
        indices.push_back(k1);
        indices.push_back(k2);
        indices.push_back(k1 + 1);
      }

      if(st != (stack_count - 1)) {
        // k1+1 => k2 => k2+1
        indices.push_back(k1+1);
        indices.push_back(k2);
        indices.push_back(k2+1);
      }
    }
  }

  std::vector<float> raw_data;
  const size_t vertex_count = positions.size() / 3;
  raw_data.reserve(vertex_count * 8);

  for(size_t i=0; i < vertex_count; ++i) {
    int pos_offset = i * 3;
    raw_data.push_back(positions[pos_offset]);
    raw_data.push_back(positions[pos_offset + 1]);
    raw_data.push_back(positions[pos_offset + 2]);

    int uv_offset = i * 2;
    raw_data.push_back(uvs[uv_offset]);
    raw_data.push_back(uvs[uv_offset + 1]);

    int normal_offset = i * 3;
    raw_data.push_back(normals[normal_offset]);
    raw_data.push_back(normals[normal_offset + 1]);
    raw_data.push_back(normals[normal_offset + 2]);

  }

  return Eng::Mesh(
    raw_data,
    indices,
    std::vector{
      Eng::VertexAttribute{0, 3, 0},
      Eng::VertexAttribute{1, 2, sizeof(float) * 3},
      Eng::VertexAttribute{2, 3, sizeof(float) * 5},
    },
    raw_data.size() / 8,
    sizeof(float) * 8
  );
}

std::ostream& operator<<(std::ostream& os, glm::vec3 v) {
  os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
  return os;
}