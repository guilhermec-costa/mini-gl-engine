#pragma once

#include "render_object.hpp"
#include "texture2d.hpp"
#include "shader.hpp"
#include "types.hpp"
#include <optional>

namespace Eng {

class Material {
  static constexpr unsigned short ALBEDO_UNIT = 0;
  static constexpr unsigned short DIFFUSE_MAP_UNIT = 7;

public:
  Material(Shader& shader): shader(shader) {};
  Material(Shader& shader, Texture2D& albedo): shader(shader), albedo(&albedo) {};
  void bind();
  void patch_uniform(std::string name, UniformValue new_value);
  std::optional<UniformValue> get_uniform(std::string name) const;
  void set_ambient(const glm::vec3 ambient);
  void set_diffuse(const glm::vec3 diffuse);
  void set_specular(const glm::vec3 specular);
  void set_shininess(const float shininess);
  void set_diffuse_map(Texture2D& diffuse_map);
  void set_light(const Light& light);

public:
  EngTypes::Color color;
  bool use_diffuse_map = false;
  Shader& shader;
  glm::vec3 ambient_light;
  Texture2D* albedo = nullptr;
  Texture2D* diffuse_map = nullptr;
  UniformMap uniforms;

private:
  float specular_strength;
  float ambient_strength;
  Material();
};
}