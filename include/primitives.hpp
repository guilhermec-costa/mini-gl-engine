#pragma once

#include "glm/ext/vector_float3.hpp"
#include "material.hpp"
#include "mesh.hpp"

namespace Eng {

class Transform {
public:
  Transform();
  void set_position(const glm::vec3& pos);
  void move(const glm::vec3& offset);
  void set_rotate(float angle, const glm::vec3& axis);
  void rotate(float angle, const glm::vec3& axis);
  void set_scale(const glm::vec3& factor);
  void scale(const glm::vec3& factor);
  glm::mat4 matrix() const;

private:
  glm::vec3 position;
  glm::vec3 scale_factor;
  glm::vec3 rotation_axis;
  float rotation_angle;
};

} // namespace Eng

Eng::Mesh make_quad(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 v4,
                    Eng::Material *material);
Eng::Mesh make_triangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3,
                        Eng::Material *material);
Eng::Mesh make_cube(Eng::Material *material);
