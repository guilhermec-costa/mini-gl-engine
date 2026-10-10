#pragma once

#include "render_object.hpp"
#include "texture2d.hpp"
#include "shader.hpp"
#include "types.hpp"
#include <optional>

namespace Eng {

class Material {
  static constexpr unsigned short ALBEDO_UNIT = 0;
  static constexpr unsigned short DIFFUSE_MAP_UNIT = 1;
  static constexpr unsigned short SPECULAR_MAP_UNIT = 2;
  static constexpr unsigned short EMISSION_MAP_UNIT = 3;

public:
  Material(Shader& shader): shader(shader) {};
  void bind();
  void patch_uniform(std::string name, UniformValue new_value);
  std::optional<UniformValue> get_uniform(std::string name) const;
  void set_ambient(const glm::vec3 ambient);
  void set_diffuse(const glm::vec3 diffuse);
  void set_specular(const glm::vec3 specular);
  void set_shininess(const float shininess);
  void set_diffuse_map(Texture2D* const diffuse_map);
  void set_specular_map(Texture2D* const specular_map);
  void set_emission_map(Texture2D* const emission_map);
  void set_emission_intensity(float intensity);
  void set_light(const Light& light);

public:
  EngTypes::Color color;
  bool use_diffuse_map = false;
  bool use_emission_map = false;
  Shader& shader;
  glm::vec3 ambient_light;
  Texture2D* diffuse_map = nullptr;
  Texture2D* specular_map = nullptr;
  Texture2D* emission_map = nullptr;
  UniformMap uniforms;

private:
  float specular_strength;
  float ambient_strength;

private:
  Material();
  void bind_maps();
};
}