#pragma once

#include "GLFW/glfw3.h"

inline bool key_pressed(GLFWwindow* window, int key) {
  if(glfwGetKey(window, key) == GLFW_PRESS) {
    return true;
  }
  return false;
}
