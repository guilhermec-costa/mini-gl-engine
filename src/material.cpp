#include "material.hpp"

namespace Eng {

void Material::bind() const {
  if (albedo) {
    albedo->bind(0);
    shader.patch_uniform("albedo", ALBEDO_UNIT);
  }

  shader.bind();
  shader.apply();
}

void Material::set_albedo(Texture2D &a) { albedo = &a; }
} // namespace Eng