#version 330 core

in vec3 normal;
in vec3 frag_pos;
in vec2 tex_coord;

struct LightMap {
  sampler2D diffuse_map;
  bool use_diffuse_map;

  sampler2D specular_map;
  bool use_specular_map;

  sampler2D emission_map;
  float emission_intensity;
  bool use_emission_map;
};

struct Material { 
  vec3 ambient;
  vec3 diffuse;
  LightMap light_map;
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

  vec3 material_ambient = material.ambient;
  vec3 material_diffuse = material.diffuse;

  if(material.light_map.use_diffuse_map) {
    vec3 tex_color = texture(material.light_map.diffuse_map, tex_coord).rgb;
    material_ambient = tex_color;
    material_diffuse = tex_color;
  }
  vec3 ambient = light.ambient * material_ambient;

  float diffuse_intensity = max(dot(norm, light_dir), 0.0);
  vec3 diffuse = light.diffuse * (diffuse_intensity * material_diffuse);


  float spec_intensity = 0.0;
  if(diffuse_intensity > 0.0) {
    vec3 view_dir = normalize(view_pos - frag_pos);
    vec3 reflect_dir = reflect(-light_dir, norm);
    spec_intensity = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
  }

  vec3 material_specular = material.specular;
  if(material.light_map.use_specular_map) {
    material_specular = texture(material.light_map.specular_map, tex_coord).rgb;
  }

  vec3 emission_value = vec3(0.0);
  if(material.light_map.use_emission_map) {
    emission_value = texture(material.light_map.emission_map, tex_coord).rgb * material.light_map.emission_intensity;
    if(length(material_specular) > 0.01 ) {
      emission_value = vec3(0.0);
    }
  }
  vec3 specular = light.specular * (spec_intensity * material_specular);
  vec3 result_color = ambient + diffuse + emission_value + specular;
  frag_color = vec4(result_color, 1.0);
}