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
  void draw(const RenderObject& object);
};

}