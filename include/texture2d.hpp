#pragma once

#include <filesystem>

#include "assimp/texture.h"

std::filesystem::path texturepath(const char* path);

namespace Engine {

class Texture2D {
 public:
  Texture2D() = default;
  Texture2D(const char* path);
  Texture2D(const unsigned char* buffer, int buffer_size);
  Texture2D(const aiTexel* data, unsigned int width, unsigned int height);
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

}  // namespace Engine

inline Engine::Texture2D make_texture(const char* path) {
  return Engine::Texture2D(texturepath(path).c_str());
}