#pragma once 

#include "render_object.hpp"

namespace Eng {
  class Light : public RenderObject {
  public:
    Light() = default;
    Light(Mesh* mesh, Material* material): RenderObject(mesh, material) {};
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
  };
}