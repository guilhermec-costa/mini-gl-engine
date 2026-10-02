#include "camera.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

namespace Eng {

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up, float fov,
               float aspect_ratio, float near_plane, float far_plane)
    : position(position), front(glm::normalize(target - position)), up(up), FOV(fov),
      aspect_ratio(aspect_ratio), near_plane(near_plane), far_plane(far_plane) {
}

glm::mat4 Camera::view_matrix() const { 
  return glm::lookAt(position, position + front, up); 
}

glm::mat4 Camera::projection() const {
  return glm::perspective(glm::radians(FOV), aspect_ratio, near_plane,
                            far_plane);
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

} // namespace Eng