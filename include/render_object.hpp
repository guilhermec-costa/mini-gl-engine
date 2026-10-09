#pragma once

#include "mesh.hpp"
#include "primitives.hpp"

namespace Eng {
class Material;

  class RenderObject {
  public:
    RenderObject() = default;
    RenderObject(Mesh* mesh, Material* material)
      : mesh(mesh), material(material) {}
  
    Eng::Mesh* mesh;
    Eng::Material* material;
    Eng::Transform transform; 
  };

class Light : public RenderObject {
public:
  Light() = default;
  Light(Mesh* mesh, Material* material): RenderObject(mesh, material) {};
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
};

}
