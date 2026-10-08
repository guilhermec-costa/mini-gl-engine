#include "input.hpp"
#include "GLFW/glfw3.h"

namespace Eng {

void Input::update() { previous = current; }

void Input::process_key(int key, int action) {
  if (action == GLFW_PRESS) {
    current[key] = GLFW_PRESS;
  } else if (action == GLFW_RELEASE) {
    current[key] = GLFW_RELEASE;
  }
}

bool Input::key_down(int key) const {
  auto it = current.find(key);
  if (it == current.end())
    return false;
  return it->second == GLFW_PRESS;
}

bool Input::key_pressed(int key) const {
  auto current_it = current.find(key);
  if (current_it == current.end())
    return false;

  auto previous_it = previous.find(key);
  int prev = previous_it == previous.end() ? GLFW_RELEASE : previous_it->second;

  return prev == GLFW_RELEASE && current_it->second == GLFW_PRESS;
}

bool Input::key_released(int key) const {
  auto current_it = current.find(key);
  if(current_it == current.end()) return false;

  auto previous_it = previous.find(key);
  int prev = previous_it == previous.end() ? GLFW_RELEASE : previous_it->second;

  return prev == GLFW_PRESS && current_it->second == GLFW_RELEASE;
}

} // namespace Eng