#include "material.hpp"

namespace Eng {

void Material::bind() {
  if (albedo) {
    albedo->bind(ALBEDO_UNIT);
    patch_uniform("albedo", ALBEDO_UNIT);
  }
  if(diffuse_map) {
    diffuse_map->bind(DIFFUSE_MAP_UNIT);
    patch_uniform("material.diffuse_map", DIFFUSE_MAP_UNIT);
    patch_uniform("material.use_diffuse_map", true);
  }

  shader.bind();
  shader.apply_external_uniforms(uniforms);
  shader.apply_internal_uniforms();
}

void Material::patch_uniform(std::string name, UniformValue new_value) {
  uniforms[name] = std::move(new_value);
}

std::optional<UniformValue> Material::get_uniform(std::string name) const {
  auto it = uniforms.find(name);
  if(it == uniforms.end()) 
    return std::nullopt;

  return it->second;
}

void Material::set_ambient(const glm::vec3 ambient) {
  patch_uniform("material.ambient", ambient);
}

void Material::set_diffuse(const glm::vec3 diffuse) {
  patch_uniform("material.diffuse", diffuse);
}

void Material::set_specular(const glm::vec3 specular) {
  patch_uniform("material.specular", specular);
}

void Material::set_shininess(const float shininess) {
  patch_uniform("material.shininess", shininess);
}

void Material::set_light(const Light& light) {
  patch_uniform("light.ambient", light.ambient);
  patch_uniform("light.diffuse", light.diffuse);
  patch_uniform("light.specular", light.specular);
  patch_uniform("light.position", light.transform.position);
}

void Material::set_diffuse_map(Texture2D& map) {
  diffuse_map = &map;
}

} // namespace Eng