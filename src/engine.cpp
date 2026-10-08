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

namespace Eng {
Controller::Controller(Window *window, Renderer* renderer, Input* input_handler) 
  : _window(window), _renderer(renderer), _input(input_handler) {};

bool Controller::should_stop() const {
  return glfwWindowShouldClose(_window->unwrap()) || quit_app;
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

  if(cursor_captured) {
    main_camera.process_input(delta_time);
  }

  if(_input->key_pressed(GLFW_KEY_TAB)) {
    polygon_mode = polygon_mode == GL_FILL ? GL_LINE : GL_FILL;
    glPolygonMode(GL_FRONT_AND_BACK,  polygon_mode);
  }
}

void Controller::process_mouse_input(GLFWwindow* window, double xpos, double ypos) {
  Controller* controller = static_cast<Controller*>(glfwGetWindowUserPointer(window));
  controller->main_camera.update_direction(xpos, ypos);
};

void Controller::process_mouse_wheel(GLFWwindow* window, double xoffset, double yoffset) {
  Controller* controller = static_cast<Controller*>(glfwGetWindowUserPointer(window));
  controller->main_camera.update_zoom(xoffset, yoffset);
}

void Controller::toggle_cursor_captured() {
  cursor_captured = !cursor_captured;
}

void Controller::render() {
  _window->clear({1.0f, 1.0f, 1.0f, 1.0f});
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

  float delta_time = 0.0f, last_frame_time = 0.0f;
  while (!should_stop()) {
    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Hello window");
    ImGui::Text("Delta time: %.4f", delta_time);
    ImGui::End();
    const float frame_time = glfwGetTime();
    delta_time = frame_time - last_frame_time;
    last_frame_time = frame_time;
    process_input(delta_time);
    update(delta_time);

    render();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(_window->unwrap());

    _input->update();
  }
}
} // namespace Eng