#pragma once

#include "types.hpp"
#include <expected>
#include <filesystem>
#include <unordered_map>
#include <variant>
#include <glm/glm.hpp>

std::filesystem::path shaderpath(const char *path);

using UniformValue = std::variant<
  int, 
  float, 
  glm::mat4,
  EngTypes::Color>;

struct Uniform {
  std::string name;
  UniformValue value;
};

const Uniform IDENTITY_MODEL_UNIFORM = Uniform{"model", glm::mat4(1.0f)};
const Uniform IDENTITY_VIEW_UNIFORM = Uniform("view", glm::mat4(1.0f));
const Uniform IDENTITY_PROJECTION_UNIFORM = Uniform("projection", glm::mat4(1.0f));
using UniformMap = std::unordered_map<std::string, UniformValue>;

namespace Eng {

class Shader {
public:
  Shader() = default;
  Shader(const Shader &) = delete;            // copy contructor
  Shader &operator=(const Shader &) = delete; // copy assignment operator
  Shader(Shader &&other) noexcept;            // move constructor
  Shader &operator=(Shader &&other) noexcept; // move assignment operator
  static std::expected<Shader, const char *> create(const char *vertex_path,
                                                    const char *frag_path);
  
  template<typename T>
  T get_uniform(std::string name) const;
  void add_uniform(std::string name, UniformValue value);
  void add_uniform(Uniform uniform);
  void patch_uniform(std::string name, UniformValue new_value);
  void set_uniformi(const char *name, int value) const;
  void set_uniformf(const char *name, float value) const;
  void set_uniformv4f(const char *name, float v1, float v2, float v3, float v4) const;
  void set_uniformmat4f(const char* name, glm::mat4 mat) const;

  virtual void bind() const;
  virtual void apply_internal_uniforms() const;
  virtual void apply_external_uniforms(const UniformMap uniforms) const;
  void update_uniform(std::string name, UniformValue value) const;
  ~Shader();

protected:
  static bool check_compilation(unsigned int id);
  static bool check_program_link_status(unsigned int program_id);
  // explict avoids casts to uints
  explicit Shader(unsigned int program_id);

protected:
  unsigned int _program_id;
  UniformMap uniforms;

private:
    int get_uniform_location(const char *name) const;
};
} // namespace Eng