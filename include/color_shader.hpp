#pragma once

#include "shader.hpp"
#include "types.hpp"

namespace Engine {

class ColorShader : public Shader {
public:
  ColorShader(unsigned int program_id);
  ColorShader(unsigned int program_id, EngineTypes::Color color);
  float red, green, blue, alpha;
  static std::expected<ColorShader, std::string>
  create_color_shader(const char *frag_path, std::optional<EngineTypes::Color> color);
  void set_color(const EngineTypes::Color color);
  void bind() const override;

public:
  EngineTypes::Color color;
};

} // namespace Engine
