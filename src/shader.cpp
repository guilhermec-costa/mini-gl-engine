#include <glad/glad.h>
#include "shader.hpp"
#include <expected>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace Eng {

Shader::Shader(unsigned int id) : _program_id(id) {};

Shader::Shader(Shader &&other) noexcept : _program_id(other._program_id) {
  other._program_id = 0;
}

Shader &Shader::operator=(Shader &&other) noexcept {
  if (this != &other) {
    if (_program_id != 0)
      glDeleteProgram(_program_id);
    _program_id = other._program_id;
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

  std::cout << vertex_code << std::endl;

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

} // namespace Eng