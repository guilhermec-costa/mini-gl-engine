#pragma once

#include <filesystem>

std::filesystem::path albedopath(const char *path);

class Albedo {
public:
  Albedo(const char *path, int internal_format, int pixel_format);
  ~Albedo();

  Albedo(const Albedo&) = delete;
  Albedo& operator=(const Albedo&) = delete;
  Albedo(Albedo&&) noexcept;
  Albedo& operator=(Albedo&&) noexcept;

  void bind_tex_unit(unsigned short int index) const;

private:
  bool generate_mipmap = true;
  int repeat_x_mode, repeat_y_mode;
  unsigned int id;
};