#pragma once
#include "model.hpp"
#include "scene.hpp"
#include "window.hpp"

class ModelPlayground : public Engine::Scene {
public:
  ModelPlayground(Engine::Window* window, Engine::Camera& camera);
  void update(float delta) override;

private:
  Engine::Model backpack;
  Engine::Model door;
};