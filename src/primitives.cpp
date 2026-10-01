#include <glad/glad.h>
#include "material.hpp"
#include "mesh.hpp"

Eng::Mesh make_quad(glm::vec3 v1, glm::vec3 v2,glm::vec3 v3, glm::vec3 v4, Eng::Material* material) {
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
    sizeof(float) * 5,
    material
  ); 
}

Eng::Mesh make_triangle(glm::vec3 v1, glm::vec3 v2,glm::vec3 v3, Eng::Material* material) {
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
    sizeof(float) * 5,
    material
  );
}