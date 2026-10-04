#pragma once

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"
namespace Eng {

class Camera {
public:
  Camera() = default;
  Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up, float initial_x, float initial_y, float fov,
         float aspect_ratio, float near_plane, float far_plane);

  glm::mat4 view_matrix() const;
  glm::mat4 projection_matrix() const; 

  void update_direction(double xpos, double ypos);
  void update_zoom(double xoffset, double yoffset);
  void move_front(float delta);
  void move_back(float delta);
  void move_right(float delta);
  void move_left(float delta);
  void move_up(float delta);
  void move_down(float delta);
  void speed_up();
  void reset_speed();

public:
  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 up;

  float fov = 45.0f;
  float aspect_ratio;
  float near_plane;
  float far_plane;
  float speed = 1.0f;
  float initial_speed = 1.0f;
  float sensitivity = 0.01f;


  bool first_mouse = true;
  float yaw = -90.0f, pitch = 0.0f;
  float last_x = 0.0f, last_y = 0.0f;

private:
    float speed_up_factor = 2.0f;
};
} // namespace Eng