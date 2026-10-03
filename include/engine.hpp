#pragma once

#include "camera.hpp"
#include "renderer.hpp"
#include "scene.hpp"
#include "window.hpp"
#include <vector>

namespace Eng {

class Controller {
public:
  Controller(const Controller &) = delete;
  Controller &operator=(const Controller &) = delete;
  Controller(Window *window, Renderer* renderer);
  void loop();

private:
  bool should_stop() const;
  void process_input(float delta_time);
  static void process_mouse_input(GLFWwindow* window, double xpos, double ypos);
  static void process_mouse_wheel(GLFWwindow* window, double xoffset, double yoffset);
  void update(float delta);
  void render();
  bool quit_app = false;
  Window *_window = nullptr;
  Renderer *_renderer = nullptr;
  Camera main_camera;
  std::vector<Scene*> scenes;
};

} // namespace Eng