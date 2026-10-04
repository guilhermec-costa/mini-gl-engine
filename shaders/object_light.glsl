#version 330 core

in vec3 normal;
in vec3 frag_pos;

uniform vec4 object_color;
uniform vec4 light_color;
uniform vec3 light_pos;

out vec4 frag_color;

void main() {
  float ambient_strength = 0.2;
  vec4 ambient = ambient_strength * light_color;

  vec3 norm = normalize(normal);
  vec3 light_dir = normalize(light_pos - frag_pos);
  float diff = max(dot(norm, light_dir), 0.0);
  vec4 diffuse = diff * light_color;
  frag_color = (ambient + diffuse) * object_color;
}