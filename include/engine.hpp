#pragma once

#include "camera.hpp"
#include "input.hpp"
#include "renderer.hpp"
#include "scene.hpp"
#include "window.hpp"
#include <vector>

namespace Eng {

class Controller {
public:
  Controller(const Controller &) = delete;
  Controller &operator=(const Controller &) = delete;
  Controller(Window *window, Renderer* renderer, Input* input);
  void loop();

private:
  static void key_callback(
    GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mods
  );

private:
  GLenum polygon_mode = GL_FILL;
  bool should_stop() const;
  void process_input(float delta_time);
  static void process_mouse_input(GLFWwindow* window, double xpos, double ypos);
  static void process_mouse_wheel(GLFWwindow* window, double xoffset, double yoffset);
  void update(float delta);
  void render();
  bool quit_app = false;
  Window *_window = nullptr;
  Renderer *_renderer = nullptr;
  Input* _input = nullptr;
  Camera main_camera;
  std::vector<Scene*> scenes;
};

} // namespace Eng