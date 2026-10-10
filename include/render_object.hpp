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
}
