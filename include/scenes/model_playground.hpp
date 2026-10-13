#pragma once
#include "scene.hpp"
#include "window.hpp"

class ModelPlayground : public Engine::Scene {
public:
  ModelPlayground(Engine::Window* window, Engine::Camera& camera);
  void update(float delta) override;
};