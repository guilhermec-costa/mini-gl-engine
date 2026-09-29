#pragma once

#include "shader.hpp"

namespace Eng {

class Material {
public:
  Material() = delete;
  Material(Shader& shader): shader(shader) {};
  void bind() const;
private:
  Shader& shader;
};
}