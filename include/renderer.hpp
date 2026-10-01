#pragma once

#include "mesh.hpp"
#include "primitives.hpp"

struct RenderObject {
  Eng::Mesh& mesh;
  Eng::Transform transform; 
};

namespace Eng {

class Renderer {
public:
  Renderer();
  void draw(const RenderObject& object);
};

}