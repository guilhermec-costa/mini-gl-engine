#include <glad/glad.h>
#include "shader.hpp"
#include <expected>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <variant>

const auto SHADER_DIR = std::filesystem::path(PROJECT_ROOT) / "shaders";
std::filesystem::path shaderpath(const char* path) {
  return (SHADER_DIR / path);
}

namespace Eng {

Shader::Shader(unsigned int id) : _program_id(id) {};

Shader::Shader(Shader &&other) noexcept 
  : _program_id(other._program_id), uniforms(std::move(other.uniforms)) {
  other._program_id = 0;
}

Shader &Shader::operator=(Shader &&other) noexcept {
  if (this != &other) {
    if (_program_id != 0)
      glDeleteProgram(_program_id);
    _program_id = other._program_id;
    uniforms = std::move(other.uniforms);
    other._program_id = 0;
  }

  return *this;
}

Shader::~Shader() {
  if (_program_id != 0) {
    glDeleteProgram(_program_id);
  }
}

std::expected<Shader, const char *> Shader::create(const char *vertex_path,
                                                   const char *frag_path) {
  std::ifstream vertexstream(vertex_path), fragstream(frag_path);
  if (!vertexstream.is_open())
    return std::unexpected("failed to open vertex shader file");
  if (!fragstream.is_open())
    return std::unexpected("failed to open fragment shader file");

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

  return Shader(ID);
};

bool Shader::check_compilation(unsigned int id) {
  int success;
  char info_log[512];
  glGetShaderiv(id, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(id, sizeof(info_log), NULL, info_log);
    std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << info_log << std::endl;
    return false;
  }

  return true;
}

bool Shader::check_program_link_status(unsigned int program_id) {
  int success;
  char info_log[512];
  glGetProgramiv(program_id, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(program_id, sizeof(info_log), NULL, info_log);
    std::cout << "ERROR::PROGRAM::LINK_FAILED\n" << info_log << std::endl;
    return false;
  }

  return true;
}

void Shader::bind() const { glUseProgram(_program_id); }

void Shader::apply() const {
  for(const auto& uniform : uniforms) {
    std::visit([&](const auto& value) {
      using T = std::decay_t<decltype(value)>;
      if constexpr (std::is_same_v<T, int>) {
        set_uniformi(uniform.name.c_str(), value);
      } else if constexpr (std::is_same_v<T, float>) {
        set_uniformf(uniform.name.c_str(), value);
      } else if constexpr (std::is_same_v<T, EngTypes::Color>) {
        const EngTypes::Color& color = value;
        set_uniformv4(
          uniform.name.c_str(),
          color.r,
          color.g,
          color.b,
          color.a
        );
      }
    }, uniform.value);
  }
}

void Shader::add_uniform(Uniform u) {
  uniforms.push_back(std::move(u));
}

void Shader::set_uniformi(const char* name, int value) const {
  int loc = glGetUniformLocation(_program_id, name);
  glUniform1i(loc, value);
}

void Shader::set_uniformf(const char* name, float value) const {
  int loc = glGetUniformLocation(_program_id, name);
  glUniform1f(loc, value);
}

void Shader::set_uniformv4(const char * name, float v1, float v2, float v3, float v4) const {
  int loc = glGetUniformLocation(_program_id, name);
  glUniform4f(loc, v1, v2, v3, v4);
}
} // namespace Eng