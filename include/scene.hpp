#pragma once

#include "camera.hpp"
#include "render_object.hpp"

namespace Engine {

class Scene {
public:
  Scene(Camera& camera)
    : camera(camera) {};
  virtual ~Scene() = default;
  virtual void update(float delta) = 0;

public:
  Camera& camera;
  std::vector<RenderObject*> objects;
};
}