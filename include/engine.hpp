#pragma once

#include "camera.hpp"
#include "renderer.hpp"
#include "window.hpp"
#include <memory>

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
  void render();
  bool quit_app = false;
  Window *_window = nullptr;
  Renderer *_renderer = nullptr;
  std::unique_ptr<Camera> camera = nullptr;
};

} // namespace Eng