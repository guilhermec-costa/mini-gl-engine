#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdint>
#include "engine.hpp"
#include "window.hpp"

void set_base_hints() {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
} 

const uint16_t WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;

int main(int argc, char* argv[]) {
  if(!glfwInit()) {
    std::cout << "failed to initialize glfw" << std::endl;
    return EXIT_FAILURE;
  }

  set_base_hints();
  std::cout << "base hints setted" << std::endl;
  Eng::Window window = Eng::Window(WINDOW_WIDTH, WINDOW_HEIGHT, "MiniGL Engine");
  if(!window.created()) {
    glfwTerminate();
    return EXIT_FAILURE;
  }

  std::cout << "application window created" << std::endl;

  glfwMakeContextCurrent(window.unwrap());

  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "failed to initialize glad" << std::endl;
    return -1;
  };

  glViewport(0, 0, window.width, window.height);
  Eng::Controller ctrl = Eng::Controller(&window);
  ctrl.loop();
  return EXIT_SUCCESS;
}