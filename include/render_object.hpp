#pragma once

#include "mesh.hpp"
#include "primitives.hpp"

namespace Engine {
class Material;

  class RenderObject {
  public:
    RenderObject() = default;
    RenderObject(Mesh* mesh, Material* material)
      : mesh(mesh), material(material) {}
  
    Engine::Mesh* mesh;
    Engine::Material* material;
    Engine::Transform transform; 
  };
}
