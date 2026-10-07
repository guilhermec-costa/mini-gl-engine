#version 330 core

in vec3 normal;
in vec3 frag_pos;

uniform float ambient_strength=0.2;
uniform float specular_strength = 0.5;
uniform vec4 object_color;
uniform vec4 light_color;
uniform vec3 light_pos;
uniform vec3 view_pos;

out vec4 frag_color;

void main() {
  vec4 ambient = ambient_strength * light_color;

  vec3 norm = normalize(normal);
  vec3 light_dir = normalize(light_pos - frag_pos);
  float diff = max(dot(norm, light_dir), 0.0);
  vec4 diffuse = diff * light_color;

  float spec = 0.0;

  if(diff > 0.0) {
    vec3 view_dir = normalize(view_pos - frag_pos);
    vec3 reflect_dir = reflect(-light_dir, norm);
    spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
  }
  vec4 specular = specular_strength * spec * light_color;
  frag_color = (ambient + diffuse + specular) * object_color;
}