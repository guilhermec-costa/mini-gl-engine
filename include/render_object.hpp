#pragma once

#include "mesh.hpp"
#include "primitives.hpp"

namespace Engine {

class Material;

class RenderObject {
 public:
  RenderObject() = default;
  RenderObject(Mesh* mesh, Material* material) : mesh(mesh), material(material) {};
  explicit RenderObject(Mesh* mesh) : mesh(mesh) {};

 public:
  Engine::Mesh* mesh = nullptr;
  Engine::Material* material = nullptr;
  Engine::Transform transform;
};

}  // namespace Engine
