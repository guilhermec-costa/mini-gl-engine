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

} // namespace Eng