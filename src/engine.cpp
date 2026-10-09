#include "engine.hpp"
#include "GLFW/glfw3.h"
#include "camera.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "input.hpp"
#include "renderer.hpp"
#include "scene.hpp"
#include "scenes/cube_scene.hpp"
#include "meshes/cube_test.hpp"
#include "scenes/light_scene.hpp"
#include <chrono>
#include <thread>

namespace Eng {

inline void sleep(double seconds) {
  std::this_thread::sleep_for(std::chrono::duration<double>(seconds));
}

Controller::Controller(Window *window, Renderer* renderer, Input* input_handler) 
  : _window(window), _renderer(renderer), _input(input_handler) {};

bool Controller::should_stop() const {
  return glfwWindowShouldClose(_window->unwrap());
};

void Controller::process_input(float delta_time) {
  if(_input->key_pressed(GLFW_KEY_ESCAPE)) {
    toggle_cursor_captured();
    glfwSetInputMode(
      _window->unwrap(), 
      GLFW_CURSOR, 
      cursor_captured ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL
    );
  }

  main_camera.process_input(delta_time);

  if(_input->key_pressed(GLFW_KEY_TAB)) {
    polygon_mode = polygon_mode == GL_FILL ? GL_LINE : GL_FILL;
    glPolygonMode(GL_FRONT_AND_BACK,  polygon_mode);
  }
}

void Controller::process_mouse_input(GLFWwindow* window, double xpos, double ypos) {
  Controller* controller = static_cast<Controller*>(glfwGetWindowUserPointer(window));
  if(controller->cursor_captured) {
    controller->main_camera.update_direction(xpos, ypos);
  } else {
    controller->main_camera.record_mouse_pos(xpos, ypos);
  }
};

void Controller::process_mouse_wheel(GLFWwindow* window, double xoffset, double yoffset) {
  Controller* controller = static_cast<Controller*>(glfwGetWindowUserPointer(window));
  controller->main_camera.update_zoom(xoffset, yoffset);
}

void Controller::toggle_cursor_captured() {
  cursor_captured = !cursor_captured;
}

void Controller::render() {
  _renderer->reset_stats();
  _window->clear();
  for(Scene* scene : scenes) {
    _renderer->render(*scene);
  }
}

void Controller::update(float delta) {
  for (Scene* scene : scenes) {
    scene->update(delta);
  }
}

void Controller::key_callback(
    GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mods
) {
  auto* controller = static_cast<Controller*>(glfwGetWindowUserPointer(window));
  controller->_input->process_key(key, action);
}

void Controller::loop() {
  glfwSetWindowUserPointer(_window->unwrap(), this);
  glfwSetInputMode(_window->unwrap(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(_window->unwrap(), Controller::process_mouse_input);
  glfwSetScrollCallback(_window->unwrap(), Controller::process_mouse_wheel);
  glfwSetKeyCallback(_window->unwrap(), Controller::key_callback);
  
  main_camera = Eng::Camera(
    _input,
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
  auto light_scene = LightScene(_window, main_camera);
  // auto geometry_scene = GeometryScene(_window, main_camera);
  // scenes.push_back(&geometry_scene);
  // scenes.push_back(&cube_scene);

  scenes.push_back(&light_scene);

  const float TARGET_FPS = 120;
  const float TARGET_DELTA_TIME = 1.0 / TARGET_FPS;
  double last_frame_time = glfwGetTime();
  while (!should_stop()) {
    double frame_start = glfwGetTime();
    double delta_time = frame_start - last_frame_time;
    last_frame_time = frame_start;

    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Debug");
    ImGui::Text("Delta time: %.4f", delta_time);
    ImGui::Text("Triangles: %d", _renderer->get_triangle());
    ImGui::Text("Draw calls: %d", _renderer->get_draw_call_count());
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::End();


    const float current_fps = 1.0 / delta_time;

    process_input(delta_time);
    update(delta_time);
    render();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(_window->unwrap());
    _input->update();
    
    double frame_elapsed = glfwGetTime() - frame_start;
    if(frame_elapsed < TARGET_DELTA_TIME) {
      sleep(TARGET_DELTA_TIME - frame_elapsed);
    }
  }
}
} // namespace Eng