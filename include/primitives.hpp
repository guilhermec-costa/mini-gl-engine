#pragma once

#include "glm/ext/vector_float3.hpp"
#include "mesh.hpp"

namespace Eng {

class Transform {
public:
  Transform() = default;
  void set_position(const glm::vec3& pos);
  void move(const glm::vec3& offset);
  void set_rotate(float angle, const glm::vec3& axis);
  void rotate(float angle, const glm::vec3& axis);
  void set_scale(const glm::vec3& factor);
  void scale(const glm::vec3& factor);
  glm::mat4 matrix() const;

public:
  glm::vec3 position{0.0f};
  glm::vec3 scale_factor{1.0f};
  glm::vec3 rotation_axis{0.0f, 0.0f, 1.0f};
  float angular_velocity{0.0f};
  float rotation_angle{0.0f};
};

} // namespace Eng

Eng::Mesh make_quad(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 v4);
Eng::Mesh make_triangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);
Eng::Mesh make_cube(const std::vector<float>& data);
Eng::Mesh make_cube_with_normals(const std::vector<float>& data);
Eng::Mesh make_sphere(float radius, int stacks, int sectors);