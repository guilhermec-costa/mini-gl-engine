#pragma once

#include "types.hpp"
#include <expected>
#include <filesystem>
#include <variant>
#include <vector>
#include <glm/glm.hpp>

std::filesystem::path shaderpath(const char *path);

using UniformValue = std::variant<int, float, EngTypes::Color>;

struct Uniform {
  std::string name;
  UniformValue value;
};

namespace Eng {

class Shader {
public:
  Shader(const Shader &) = delete;            // copy contructor
  Shader &operator=(const Shader &) = delete; // copy assignment operator
  Shader(Shader &&other) noexcept;            // move constructor
  Shader &operator=(Shader &&other) noexcept; // move assignment operator
  static std::expected<Shader, const char *> create(const char *vertex_path,
                                                    const char *frag_path);

  void add_uniform(const Uniform u);
  void set_uniformi(const char *name, int value) const;
  void set_uniformf(const char *name, float value) const;
  void set_uniformv4(const char *name, float v1, float v2, float v3, float v4) const;
  virtual void bind() const;
  virtual void apply() const;
  ~Shader();

protected:
  static bool check_compilation(unsigned int id);
  static bool check_program_link_status(unsigned int program_id);
  // explict avoids casts to uints
  explicit Shader(unsigned int program_id);

protected:
  unsigned int _program_id;

private:
    std::vector<Uniform> uniforms;
};
} // namespace Eng