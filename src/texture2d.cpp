#include <glad/glad.h>
#include "texture2d.hpp"
#include "vendor/stb_image.h"

const std::filesystem::path ALBEDO_PATH = std::filesystem::path(PROJECT_ROOT) / "assets/albedos";
std::filesystem::path albedopath(const char *path) {
  return std::filesystem::path(ALBEDO_PATH) / path;
}

namespace Eng {

Texture2D::Texture2D(const char *path, int internal_format, int pixel_format) 
: repeat_x_mode(GL_REPEAT), repeat_y_mode(GL_REPEAT) {
  glGenTextures(1, &id);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, id);

  int w, h, nrch;
  unsigned char *data = stbi_load(path, &w, &h, &nrch, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, w, h, 0, pixel_format,
                  GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
  }

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  if (generate_mipmap) {
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  }

  glBindTexture(GL_TEXTURE_2D, 0);
}

Texture2D::Texture2D(Texture2D &&other) noexcept : id(other.id) { other.id = 0; }

Texture2D &Texture2D::operator=(Texture2D &&other) noexcept {
  if (this != &other) {
    if (id != 0)
      glDeleteTextures(1, &id);

    id = other.id;
    other.id = 0;
  }
  return *this;
}

Texture2D::~Texture2D() { 
  if(id != 0)
    glDeleteTextures(1, &id); 
}

void Texture2D::bind(unsigned short index) const {
  glActiveTexture(GL_TEXTURE0 + index);
  glBindTexture(GL_TEXTURE_2D, id);
}
}