#include <glad/glad.h>
#include "shader.hpp"
#include "glm/ext/vector_float4.hpp"
#include <expected>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <variant>
#include <glm/gtc/type_ptr.hpp>

const auto SHADER_DIR = std::filesystem::path(PROJECT_ROOT) / "shaders";
std::filesystem::path shaderpath(const char* path) {
  return (SHADER_DIR / path);
}

namespace Eng {

Shader::Shader(unsigned int id) : _program_id(id) {
  add_uniform(IDENTITY_MODEL_UNIFORM);
  add_uniform(IDENTITY_VIEW_UNIFORM);
  add_uniform(IDENTITY_PROJECTION_UNIFORM);
};

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

void Shader::update_uniform(std::string name, UniformValue value) const {
    std::visit([&](const auto& value) {
      using T = std::remove_cvref_t<decltype(value)>;
      if constexpr (std::is_same_v<T, int>) {
        set_uniformi(name.c_str(), value);
      } else if constexpr (std::is_same_v<T, float>) {
        set_uniformf(name.c_str(), value);
      } else if constexpr (std::is_same_v<T, glm::vec3>) {
        const glm::vec3& v = value;
        set_uniformv3f(name.c_str(), v.x, v.y, v.z);
      } else if constexpr (std::is_same_v<T, glm::mat4>) {
        const glm::mat4& m = value;
        set_uniformmat4f(name.c_str(), m);
      } else if constexpr(std::is_same_v<T, glm::vec4>) {
        const glm::vec4& color = value;
        set_uniformv4f(
          name.c_str(),
          color.r,
          color.g,
          color.b,
          color.a
        );
      } else if constexpr (std::is_same_v<T, EngTypes::Color>) {
        const EngTypes::Color& color = value;
        set_uniformv4f(
          name.c_str(),
          color.r,
          color.g,
          color.b,
          color.a
        );
      }
    }, value);
}

void Shader::add_uniform(std::string name, UniformValue value) {
  uniforms.emplace(std::move(name), std::move(value));
}

void Shader::add_uniform(Uniform u) {
  uniforms.emplace(std::move(u.name), std::move(u.value));
}

void Shader::apply_external_uniforms(const UniformMap external) const {
  for(const auto& [name, value]: external)
    update_uniform(name, value);
}

void Shader::apply_internal_uniforms() const {
  for(const auto& [name, value] : uniforms)
    update_uniform(name, value);
}

void Shader::patch_uniform(std::string name, UniformValue new_value) {
  uniforms.at(name) = new_value;
}


int Shader::get_uniform_location(const char* name) const {
  return glGetUniformLocation(_program_id, name);
}

void Shader::set_uniformi(const char* name, int value) const {
  glUniform1i(get_uniform_location(name), value);
}

void Shader::set_uniformf(const char* name, float value) const {
  glUniform1f(get_uniform_location(name), value);
}

void Shader::set_uniformv3f(const char* name, float v1, float v2, float v3) const {
  glUniform3f(get_uniform_location(name), v1, v2, v3);
}

void Shader::set_uniformv4f(const char * name, float v1, float v2, float v3, float v4) const {
  glUniform4f(get_uniform_location(name), v1, v2, v3, v4);
}

void Shader::set_uniformmat4f(const char* name, const glm::mat4 mat) const {
  glUniformMatrix4fv(get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(mat));
}
} // namespace Eng