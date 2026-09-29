#include "material.hpp"

namespace Eng {

void Material::bind() const {
  const unsigned short unit = 0;
  if (albedo) {
    albedo->bind_tex_unit(unit);
  }

  shader.bind();
  shader.set_uniformi("albedo", unit);
}

void Material::set_albedo(Albedo &a) { albedo = &a; }
} // namespace Eng