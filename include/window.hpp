#pragma once

#include "GLFW/glfw3.h"
#include <cstdint>
#include "types.hpp"

namespace Eng {

class Window {
public:
  uint16_t width, height;
  EngTypes::Color clear_color = {0.0f, 0.0f, 0.0f, 1.0f};

public:
  Window(uint16_t w, uint16_t h, const char *title);
  Window(const Window &) = delete;
  ~Window();

  inline bool created() const { return _window != NULL; }
  inline GLFWwindow *unwrap() const { return _window; };

  float aspect_ratio() const;
  void clear() const;
  void close();

  inline void set_clear_color(const EngTypes::Color color) {
    clear_color = color;
  }

private:
  GLFWwindow *_window;
};
} // namespace Eng