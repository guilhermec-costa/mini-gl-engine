#pragma once

#include "scene.hpp"

namespace Eng {

class Renderer {
public:
  Renderer();
  void draw(const RenderObject& object, const Camera& camera);
  void render(Scene& scene);
};

}