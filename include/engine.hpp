#pragma once

#include "window.hpp"
namespace Eng {
class Controller {
public:
  Controller(const Controller&) = delete;
  Controller& operator=(const Controller&) = delete;
  Controller(Window* window);
  void loop();

private:
  bool should_stop() const;
  Window* _window;
};
}