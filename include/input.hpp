#pragma once

#include "GLFW/glfw3.h"
#include <unordered_map>

static std::unordered_map<int, int> previous;

inline bool key_pressed(GLFWwindow *window, int key) {
  if (glfwGetKey(window, key) == GLFW_PRESS) {
    return true;
  }
  return false;
}

inline bool key_released(GLFWwindow *window, int key) {
  int current = glfwGetKey(window, key);
  bool released = previous[key] == GLFW_PRESS && current == GLFW_RELEASE;
  previous[key] = current;
  return released;
}
