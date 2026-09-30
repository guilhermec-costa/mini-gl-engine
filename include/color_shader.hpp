#pragma once

#include "shader.hpp"
#include "types.hpp"

namespace Eng {

class ColorShader : public Shader {
public:
  ColorShader(unsigned int program_id);
  ColorShader(unsigned int program_id, EngTypes::Color color);
  float red, green, blue, alpha;
  static std::expected<ColorShader, const char *>
  create_color_shader(const char *frag_path, std::optional<EngTypes::Color> color);
  void set_color(const EngTypes::Color color);
  void bind() const override;

public:
  EngTypes::Color color;
};

} // namespace Eng
