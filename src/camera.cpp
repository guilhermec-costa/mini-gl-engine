#include "camera.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

namespace Eng {

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up, 
              float initial_x, float initial_y, 
              float fov, float aspect_ratio, float near_plane, float far_plane)
    : position(position), front(glm::normalize(target - position)), up(up),
      last_x(initial_x), last_y(initial_y),
      fov(fov), aspect_ratio(aspect_ratio), near_plane(near_plane), far_plane(far_plane) {
}

glm::mat4 Camera::view_matrix() const { 
  return glm::lookAt(position, position + front, up); 
}

glm::mat4 Camera::projection_matrix() const {
  return glm::perspective(glm::radians(fov), aspect_ratio, near_plane,
                            far_plane);
}

void Camera::update_direction(double xpos, double ypos) {
  if(first_mouse) {
    last_x = xpos;
    last_y = ypos;
    first_mouse = false;
  }

  float xoffset = xpos - last_x;
  float yoffset = last_y - ypos;
  last_x = xpos;
  last_y = ypos;

  xoffset *= sensitivity;
  yoffset *= sensitivity;
  yaw += xoffset;
  pitch += yoffset;

  if(pitch > 89.0f)
    pitch = 89.0f;
  if(pitch < -89.0f)
    pitch = -89.0f;

  glm::vec3 direction;
  direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  direction.y = sin(glm::radians(pitch));
  direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  front = glm::normalize(direction);
}

void Camera::update_zoom(double xoffset, double yoffset) {
  fov -= (float)yoffset;
  if(fov < 1.0f) {
    fov = 1.0f;
  }
  if(fov > 45.0f) {
    fov = 45.0f;
  }
}

void Camera::move_front(float delta) {
  position += speed * front * delta;
}

void Camera::move_back(float delta) {
  position -= speed * front * delta;
}

void Camera::move_right(float delta) {
  position += glm::normalize(glm::cross(front, up)) * speed * delta;
}

void Camera::move_left(float delta) {
  position -= glm::normalize(glm::cross(front, up)) * speed * delta;
}

void Camera::move_up(float delta) {
  position.y += speed * delta; 
}

void Camera::move_down(float delta) {
  position.y -= speed * delta;
}

void Camera::speed_up() {
  speed *= speed_up_factor;
}

void Camera::reset_speed() {
  speed = initial_speed;
}

} // namespace Eng