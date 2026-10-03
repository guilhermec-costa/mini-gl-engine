#pragma once

#include "texture2d.hpp"
#include "shader.hpp"
#include "types.hpp"

namespace Eng {

class Material {
  static constexpr unsigned short ALBEDO_UNIT = 0;

public:
  Material(Shader& shader): shader(shader) {};
  Material(Shader& shader, Texture2D& albedo): shader(shader), albedo(&albedo) {};
  void bind();
  void set_albedo(Texture2D& albedo);
  void add_uniform(Uniform u);
  void patch_uniform(std::string name, UniformValue new_value);

public:
  EngTypes::Color color;
  Shader& shader;
  Texture2D* albedo = nullptr;
  UniformMap uniforms;

private:
  Material();
};
}