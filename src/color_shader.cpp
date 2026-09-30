#include <glad/glad.h>
#include "color_shader.hpp"
#include "shader.hpp"
#include <fstream>

namespace Eng {

ColorShader::ColorShader(unsigned int program_id)
    : Shader(program_id), color({0.0f, 0.0f, 0.0f, 1.0f}) {
  add_uniform({"color", color});
};

ColorShader::ColorShader(unsigned int program_id, EngTypes::Color color)
    : Shader(program_id), color(color) {
  add_uniform({"color", color});
};

std::expected<ColorShader, const char *>
ColorShader::create_color_shader(const char *frag_path,
                                 std::optional<EngTypes::Color> color) {

  std::filesystem::path vp = shaderpath("default_vertex.glsl");
  const char *vertex_path = vp.c_str();

  std::ifstream vertexstream(vertex_path), fragstream(frag_path);
  if (!vertexstream.is_open())
    return std::unexpected("failed to open default vertex shader file");
  if (!fragstream.is_open())
    return std::unexpected("failed to open base fragment color shader file");

  std::stringstream vertexss, fragss;
  vertexss << vertexstream.rdbuf();
  fragss << fragstream.rdbuf();

  std::string vertexsrc = vertexss.str();
  std::string fragsrc = fragss.str();

  const char *vertex_code = vertexsrc.c_str();
  const char *frag_code = fragsrc.c_str();

  unsigned int vertex_shader, fragment_shader;
  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_code, NULL);
  glCompileShader(vertex_shader);

  if (!Shader::check_compilation(vertex_shader)) {
    glDeleteShader(vertex_shader);
    return std::unexpected("failed to compile vertex shader");
  }

  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &frag_code, NULL);
  glCompileShader(fragment_shader);

  if (!Shader::check_compilation(fragment_shader)) {
    glDeleteShader(fragment_shader);
    return std::unexpected("failed to compile fragment shader");
  }

  unsigned int ID = glCreateProgram();
  glAttachShader(ID, vertex_shader);
  glAttachShader(ID, fragment_shader);

  glLinkProgram(ID);

  if (!Shader::check_program_link_status(ID)) {
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    glDeleteProgram(ID);
    return std::unexpected("failed to link program");
  }

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  if (color.has_value())
    return ColorShader(ID, color.value());
  return ColorShader(ID);
}

void ColorShader::set_color(const EngTypes::Color c) { color = c; }

void ColorShader::bind() const { glUseProgram(_program_id); }
} // namespace Eng