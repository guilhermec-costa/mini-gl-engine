#pragma once

#include <vendor/glad/glad.h>

#include "scene.hpp"

namespace Engine {

struct RenderMetadata {
  const GLubyte* gpu_model;
  const GLubyte* vendor;
  const GLubyte* version;
};

class Renderer {
 public:
  Renderer();
  void draw(const RenderObject& object, const Camera& camera);
  void render_scene(Scene& scene);
  void reset_stats();
  int get_triangle() const;
  int get_draw_call_count() const;
  RenderMetadata get_metadata() const;

 private:
  int triangle_count = 0;
  int draw_call_count = 0;
};

}  // namespace Engine