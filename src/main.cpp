#include <vendor/glad/glad.h>

#include <GLFW/glfw3.h>

#include <cstdint>
#include <iostream>

#include "engine.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "macros.hpp"
#include "renderer.hpp"
#include "resource/shader_storage.hpp"
#include "resource/texture_storage.hpp"
#include "window.hpp"

using namespace Engine;

void set_base_hints() {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

const uint16_t WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;

int main(int argc, char* argv[]) {
  if (!glfwInit()) {
    std::cout << "failed to initialize glfw" << std::endl;
    return EXIT_FAILURE;
  }

  set_base_hints();
  std::cout << "base hints setted" << std::endl;

  {
    Window window = Window(WINDOW_WIDTH, WINDOW_HEIGHT, "MiniGL Engine");
    if (!window.created()) {
      glfwTerminate();
      return EXIT_FAILURE;
    }

    std::cout << "application window created" << std::endl;

    glfwMakeContextCurrent(window.unwrap());

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::cout << "failed to initialize glad" << std::endl;
      return -1;
    };

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window.unwrap(), true);
    ImGui_ImplOpenGL3_Init(nullptr);

    glViewport(0, 0, window.width, window.height);
    Renderer renderer = Renderer();
    Input input_handler = Input();

    window.set_clear_color({.0f, .0f, .0f, 1.f, false});

    TEXTURES.load(texturepath("wall.jpg"));
    TEXTURES.load(texturepath("wood_square.png"));
    TEXTURES.load(texturepath("wood.jpg"));
    TEXTURES.load(texturepath("matrix.jpg"));
    TEXTURES.load(texturepath("box_specular_map.png"));
    TEXTURES.load(texturepath("earth.jpeg"));
    TEXTURES.load(texturepath("sun.jpg"));

    SHADERS.load(ShaderId::Base, "base/base_vertex.glsl", "base/base_fragment.glsl");

    Controller ctrl = Controller(&window, &renderer, &input_handler);
    ctrl.loop();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  }

  glfwTerminate();
  return EXIT_SUCCESS;
}