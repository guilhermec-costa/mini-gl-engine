#pragma once

#include <expected>
#include <filesystem>

std::filesystem::path shaderpath(const char *path);

namespace Eng {

class Shader {
public:
  Shader(const Shader &) = delete;            // copy contructor
  Shader &operator=(const Shader &) = delete; // copy assignment operator
  Shader(Shader &&other) noexcept;            // move constructor
  Shader &operator=(Shader &&other) noexcept; // move assignment operator
  static std::expected<Shader, const char *> create(const char *vertex_path,
                                                    const char *frag_path);

  void set_uniformi(const char *name, int value) const;
  void bind() const;
  ~Shader();

private:
  static bool check_compilation(unsigned int id);
  static bool check_program_link_status(unsigned int program_id);
  // explict avoids casts to uints
  explicit Shader(unsigned int program_id);

private:
  unsigned int _program_id;
};

} // namespace Eng