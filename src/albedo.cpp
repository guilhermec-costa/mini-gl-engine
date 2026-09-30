#include <glad/glad.h>
#include "albedo.hpp"
#include "vendor/stb_image.h"

const std::filesystem::path ALBEDO_PATH = std::filesystem::path(PROJECT_ROOT) / "assets/albedos";
std::filesystem::path albedopath(const char *path) {
  return std::filesystem::path(ALBEDO_PATH) / path;
}

Albedo::Albedo(const char *path, int internal_format, int pixel_format) 
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

Albedo::Albedo(Albedo &&other) noexcept : id(other.id) { other.id = 0; }

Albedo &Albedo::operator=(Albedo &&other) noexcept {
  if (this != &other) {
    if (id != 0)
      glDeleteTextures(1, &id);

    id = other.id;
    other.id = 0;
  }
  return *this;
}

Albedo::~Albedo() { 
  if(id != 0)
    glDeleteTextures(1, &id); 
}

void Albedo::bind_tex_unit(unsigned short index) const {
  glActiveTexture(GL_TEXTURE0 + index);
  glBindTexture(GL_TEXTURE_2D, id);
}