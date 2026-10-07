#include "material.hpp"
#include "glm/ext/vector_float4.hpp"

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

void Material::set_specular_strength(float strength) {
  patch_uniform("specular_strength", strength);
}

void Material::set_ambient_strength(float strength) {
  patch_uniform("ambient_strength", strength);
}

void Material::set_color(glm::vec4 color) {
 patch_uniform("object_color", color);
}

void Material::set_albedo(Texture2D &a) { albedo = &a; }
} // namespace Eng