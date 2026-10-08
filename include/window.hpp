#pragma once

#include "GLFW/glfw3.h"
#include <cstdint>
#include "primitives.hpp"
#include "types.hpp"

namespace Eng {

class Window {
public:
  uint16_t width, height;

public:
  Window(uint16_t w, uint16_t h, const char *title);
  Window(const Window &) = delete;
  ~Window();

  float aspect_ratio() const;
  inline GLFWwindow *unwrap() const { return _window; };
  void clear(const EngTypes::Color clear_color) const;
  inline bool created() const { return _window != NULL; }
  void close();

private:
  GLFWwindow *_window;
};
} // namespace Eng