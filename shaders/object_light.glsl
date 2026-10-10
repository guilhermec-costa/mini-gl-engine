#version 330 core

in vec3 normal;
in vec3 frag_pos;
in vec2 tex_coord;

struct Material { 
  vec3 ambient;
  vec3 diffuse;
  sampler2D diffuse_map;
  bool use_diffuse_map;
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
  vec3 norm = normalize(normal);
  vec3 light_dir = normalize(light.position - frag_pos);
  float diff = max(dot(norm, light_dir), 0.0);

  vec3 material_ambient = material.ambient;
  vec3 material_diffuse = material.diffuse;

  if(material.use_diffuse_map) {
    vec3 tex_color = texture(material.diffuse_map, tex_coord).rgb;
    material_ambient = tex_color;
    material_diffuse = tex_color;
  }
  vec3 ambient = light.ambient * material_ambient;
  vec3 diffuse = light.diffuse * (diff * material_diffuse);

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