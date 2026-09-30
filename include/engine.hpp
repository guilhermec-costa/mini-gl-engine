#pragma once

#include "renderer.hpp"
#include "window.hpp"

namespace Eng {

class Controller {
public:
  Controller(const Controller &) = delete;
  Controller &operator=(const Controller &) = delete;
  Controller(Window *window, Renderer* renderer);
  void loop();

private:
  bool should_stop() const;
  void process_events();
  void render();
  bool quit_app = false;
  Window *_window = nullptr;
  Renderer *_renderer = nullptr;
};

} // namespace Eng