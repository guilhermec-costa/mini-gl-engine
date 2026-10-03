#pragma once

#include <vector>
#include "camera.hpp"
#include "material.hpp"
#include "mesh.hpp"
#include "primitives.hpp"

namespace Eng {

class RenderObject {
public:
  RenderObject() = default;
  RenderObject(Mesh* mesh, Material* material)
    : mesh(mesh), material(material) {}

  Eng::Mesh* mesh;
  Eng::Material* material;
  Eng::Transform transform; 
};

class Scene {
public:
  Scene(Camera& camera)
    : camera(camera) {};
  virtual ~Scene() = default;
  virtual void update(float delta) = 0;
  virtual void prepare_render() = 0;

public:
  Camera& camera;
  std::vector<RenderObject*> objects;
};
}