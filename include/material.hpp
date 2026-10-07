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
  void patch_uniform(std::string name, UniformValue new_value);
  void set_specular_strength(float strength);
  void set_ambient_strength(float strength);
  void set_color(glm::vec4 color);

public:
  EngTypes::Color color;
  Shader& shader;
  glm::vec3 ambient_light;
  Texture2D* albedo = nullptr;
  UniformMap uniforms;

private:
  float specular_strength;
  float ambient_strength;
  Material();
};
}