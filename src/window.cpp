#include "window.hpp"
#include "GLFW/glfw3.h"
#include <iostream>

namespace Eng {

void resizeCallback(GLFWwindow *window, int width, int height) {
  std::cout << "resizing window to w: " << width << " h: " << height << "\n";
  glViewport(0, 0, width, height);
}

Window::Window(uint16_t w, uint16_t h, const char *title)
    : width(w), height(h) {
  _window = glfwCreateWindow(w, h, title, NULL, NULL);
  if(!_window) {
    std::cout << "failed to create glfw window" << std::endl;
  } else {
    glfwSetFramebufferSizeCallback(_window, resizeCallback);
  }
}

float Window::aspect_ratio() const {
  return static_cast<float>(width) / static_cast<float>(height);
}
void Window::clear(const EngTypes::Color color) const {
  glClearColor(color.r, color.g, color.b, color.a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
} // namespace Eng