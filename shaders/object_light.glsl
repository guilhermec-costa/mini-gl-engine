#version 330 core

in vec3 normal;
in vec3 frag_pos;

struct Material { 
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

struct Light {
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

uniform vec3 view_pos;
uniform Material material;
uniform Light light;

out vec4 frag_color;

void main() {
  vec3 ambient =  light.ambient * material.ambient;

  vec3 norm = normalize(normal);
  vec3 light_dir = normalize(light.position - frag_pos);
  float diff = max(dot(norm, light_dir), 0.0);
  vec3 diffuse = light.diffuse * (diff * material.diffuse);

  float spec = 0.0;

  if(diff > 0.0) {
    vec3 view_dir = normalize(view_pos - frag_pos);
    vec3 reflect_dir = reflect(-light_dir, norm);
    spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
  }
  vec3 specular = light.specular * (spec * material.specular);
  vec3 result_color = ambient + diffuse + specular;
  frag_color = vec4(result_color, 1.0);
}