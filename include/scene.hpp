#pragma once

#include "renderer.hpp"
namespace Eng {

class Scene {
  virtual ~Scene() = default;


  void virtual render(Renderer& renderer) = 0;
};
}