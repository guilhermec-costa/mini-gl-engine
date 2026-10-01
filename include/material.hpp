#pragma once

#include "texture2d.hpp"
#include "shader.hpp"

namespace Eng {

class Material {
  static constexpr unsigned short ALBEDO_UNIT = 0;

public:
  Material(Shader& shader): shader(shader) {};
  Material(Shader& shader, Texture2D& albedo): shader(shader), albedo(&albedo) {};
  void bind() const;
  void set_albedo(Texture2D& albedo);

public:
  Shader& shader;
  Texture2D* albedo = nullptr;

private:
  Material();
};
}