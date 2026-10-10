#pragma once

#include <filesystem>

std::filesystem::path albedopath(const char *path);

namespace Eng {

class Texture2D {
public:
  Texture2D() = default;
  Texture2D(const char *path);
  ~Texture2D();

  Texture2D(const Texture2D&) = delete;
  Texture2D& operator=(const Texture2D&) = delete;
  Texture2D(Texture2D&&) noexcept;
  Texture2D& operator=(Texture2D&&) noexcept;

  void bind(unsigned short int index) const;

private:
  bool generate_mipmap = true;
  int repeat_x_mode, repeat_y_mode;
  unsigned int id;
};

}

inline Eng::Texture2D make_texture(const char *path) {
  return Eng::Texture2D(albedopath(path).c_str());
}