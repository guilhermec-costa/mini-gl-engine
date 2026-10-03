#include "material.hpp"

namespace Eng {

void Material::bind() {
  if (albedo) {
    albedo->bind(ALBEDO_UNIT);
    patch_uniform("albedo", ALBEDO_UNIT);
  }

  shader.bind();
  shader.apply_external_uniforms(uniforms);
  shader.apply_internal_uniforms();
}

void Material::patch_uniform(std::string name, UniformValue new_value) {
  uniforms[name] = std::move(new_value);
}

void Material::add_uniform(Uniform u) {
  uniforms.emplace(std::move(u.name), std::move(u.value));
}

void Material::set_albedo(Texture2D &a) { albedo = &a; }
} // namespace Eng