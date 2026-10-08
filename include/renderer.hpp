#pragma once

#include "scene.hpp"

namespace Eng {

class Renderer {
public:
  Renderer();
  void draw(const RenderObject& object, const Camera& camera);
  void render(Scene& scene);
  void reset_stats();
  int get_triangle() const;
  int get_draw_call_count() const;

private:
  int triangle_count = 0;
  int draw_call_count = 0;
};

}