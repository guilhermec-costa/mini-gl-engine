#include "engine.hpp"
#include "GLFW/glfw3.h"
#include "camera.hpp"
#include "input.hpp"
#include "renderer.hpp"
#include "scene.hpp"
#include "scenes/cube_scene.hpp"
#include "meshes/cube_test.hpp"
#include "scenes/geometry_scene.hpp"

namespace Eng {
Controller::Controller(Window *window, Renderer* renderer) 
  : _window(window), _renderer(renderer) {};

bool Controller::should_stop() const {
  return glfwWindowShouldClose(_window->unwrap()) || quit_app;
};

void Controller::process_input(float delta_time) {
  GLFWwindow* window = _window->unwrap();
  if(key_pressed(window, GLFW_KEY_ESCAPE)) {
    quit_app = true;
  }
  if(key_pressed(window, GLFW_KEY_W)) main_camera.move_front(delta_time);
  if(key_pressed(window, GLFW_KEY_S)) main_camera.move_back(delta_time);
  if(key_pressed(window, GLFW_KEY_A)) main_camera.move_left(delta_time);
  if(key_pressed(window, GLFW_KEY_D)) main_camera.move_right(delta_time);
  if(key_pressed(window, GLFW_KEY_E)) main_camera.move_up(delta_time);
  if(key_pressed(window, GLFW_KEY_Q)) main_camera.move_down(delta_time);
}

void Controller::process_mouse_input(GLFWwindow* window, double xpos, double ypos) {
  Controller* controller = static_cast<Controller*>(glfwGetWindowUserPointer(window));
  controller->main_camera.update_direction(xpos, ypos);
};

void Controller::process_mouse_wheel(GLFWwindow* window, double xoffset, double yoffset) {
  Controller* controller = static_cast<Controller*>(glfwGetWindowUserPointer(window));
  controller->main_camera.update_zoom(xoffset, yoffset);
}

void Controller::render() {
  _window->clear({0.0f, 50.0f, 128.f, 1.0f});
  for(Scene* scene : scenes) {
    _renderer->render(*scene);
  }
  glfwSwapBuffers(_window->unwrap());
}

void Controller::update(float delta) {
  for (Scene* scene : scenes) {
    scene->update(delta);
  }
}

void Controller::loop() {
  glfwSetWindowUserPointer(_window->unwrap(), this);
  glfwSetInputMode(_window->unwrap(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(_window->unwrap(), Controller::process_mouse_input);
  glfwSetScrollCallback(_window->unwrap(), Controller::process_mouse_wheel);
  
  main_camera = Eng::Camera(
    glm::vec3(0.0f, 0.0f, 3.0f), 
    glm::vec3(0.0f, 0.0f, 0.0f), 
    glm::vec3(0.0f, 1.0f, 0.0f),
    _window->width/2.0f,
    _window->height/2.0f,
    45.f,
    _window->aspect_ratio(),
    0.1f, 100.f
  );

  auto cube_scene = CubeScene(_window, main_camera);
  auto geometry_scene = GeometryScene(_window, main_camera);
  scenes.push_back(&geometry_scene);
  scenes.push_back(&cube_scene);

  float delta_time = 0.0f, last_frame_time = 0.0f;
  while (!should_stop()) {
    const float frame_time = glfwGetTime();
    delta_time = frame_time - last_frame_time;
    last_frame_time = frame_time;

    process_input(delta_time);
    update(delta_time);
    render();
    glfwPollEvents();
  }
}
} // namespace Eng