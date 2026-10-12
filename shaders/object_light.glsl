#version 330 core

in vec3 normal;
in vec3 frag_pos;
in vec2 tex_coord;

uniform bool use_directional_light;
uniform bool use_point_light;
uniform bool use_spotlight;

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

struct DirectionalLight {
  vec3 direction;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct PointLight {
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
  float radius;
};
#define NR_POINT_LIGHTS 4

struct Spotlight {
  vec3 position;
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
  float radius;

  float cut_off;
  float outer_cut_off;
};

uniform vec3 view_pos;
uniform Material material;
uniform PointLight point_light;
uniform DirectionalLight directional_light;
uniform Spotlight spotlight;

out vec4 frag_color;

vec3 calculate_directional_light(
  DirectionalLight light, 
  vec3 norm, 
  vec3 view_dir,
  vec3 material_ambient,
  vec3 material_diffuse,
  vec3 material_specular
) {
  vec3 light_dir = normalize(-light.direction);

  float diffuse_intensity = max(dot(norm, light_dir), 0.0);
  vec3 diffuse = light.diffuse * diffuse_intensity * material_diffuse;

  float spec_intensity = 0.0;

  if (diffuse_intensity > 0.0) {
    vec3 reflect_dir = reflect(-light_dir, norm);
    spec_intensity = pow(
        max(dot(view_dir, reflect_dir), 0.0),
        material.shininess
    );
  }

  vec3 ambient = light.ambient * material_ambient;
  vec3 specular = light.specular * spec_intensity * material_specular;

  return ambient + diffuse + specular;
};

vec3 calculate_point_light(
  PointLight light, 
  vec3 norm, 
  vec3 frag_pos,
  vec3 view_dir,
  vec3 material_ambient,
  vec3 material_diffuse,
  vec3 material_specular
) {
  vec3 light_dir = normalize(light.position - frag_pos);
  float distance = length(light.position - frag_pos);

  if(distance >= light.radius) {
    return vec3(0.0);
  }

  float diffuse_intensity = max(dot(norm, light_dir), 0.0);
  vec3 diffuse = light.diffuse * diffuse_intensity * material_diffuse;

  float spec_intensity = 0.0;
  if(diffuse_intensity > 0.0) {
    vec3 reflect_dir = reflect(-light_dir, norm);
    spec_intensity = pow(
      max(dot(view_dir, reflect_dir), 0.0),
      material.shininess
    );
  }

  vec3 specular = light.specular * spec_intensity * material_specular;
  float attenuation = 1.0 /
    (light.constant + 
      light.linear * distance + 
      light.quadratic * distance * distance);

  float radius_fade = 
    1.0 - smoothstep(
      light.radius * 0.8, 
      light.radius, 
      distance
    );

  attenuation *= radius_fade;
  vec3 ambient = light.ambient * material_ambient;
  return (ambient + diffuse + specular) * attenuation;
};

vec3 calculate_spotlight(
  Spotlight light,
  vec3 norm, 
  vec3 frag_pos,
  vec3 view_dir,
  vec3 material_ambient,
  vec3 material_diffuse,
  vec3 material_specular
) {
  vec3 light_dir = normalize(light.position - frag_pos);
  float distance = length(light.position - frag_pos);

  if(distance >= light.radius) {
    return vec3(0.0);
  }

  float theta = dot(light_dir, normalize(-light.direction));
  if(theta < light.outer_cut_off) {
    return vec3(0.0);
  }

  float epsilon = light.cut_off - light.outer_cut_off;
  float cone_intensity = clamp((theta - light.outer_cut_off) / epsilon, 0.0, 1.0);    
  
  float diffuse_intensity = max(dot(norm, light_dir),0.0);
  vec3 diffuse = light.diffuse * diffuse_intensity * material_diffuse;

  float spec_intensity = 0.0;
  if (diffuse_intensity > 0.0) {
    vec3 reflect_dir = reflect(-light_dir, norm);

    spec_intensity = pow(
      max(dot(view_dir, reflect_dir), 0.0),
      material.shininess
    );
  }

  vec3 specular = light.specular * spec_intensity * material_specular;
  vec3 ambient = light.ambient * material_ambient;
  float attenuation =
    1.0 /
    (
      light.constant +
      light.linear * distance +
      light.quadratic * distance * distance
    );

  float radius_fade =
    1.0 - smoothstep(
      light.radius * 0.8,
      light.radius,
      distance
    );

  attenuation *= radius_fade;

  return (ambient + diffuse + specular) * attenuation * cone_intensity;

}

void main() {
  vec3 norm = normalize(normal);
  vec3 view_dir = normalize(view_pos - frag_pos);

  vec3 material_ambient = material.ambient;
  vec3 material_diffuse = material.diffuse;

  if(material.light_map.use_diffuse_map) {
    vec3 tex_color = texture(material.light_map.diffuse_map, tex_coord).rgb;
    material_ambient = tex_color;
    material_diffuse = tex_color;
  }

  vec3 material_specular = material.specular;
  if(material.light_map.use_specular_map) {
    material_specular = texture(material.light_map.specular_map, tex_coord).rgb;
  }

  vec3 result = vec3(0.0);

  if(use_directional_light) {
    result += calculate_directional_light(
      directional_light,
      norm,
      view_dir,
      material_ambient, 
      material_diffuse, 
      material_specular
    );
  }

  if(use_point_light) {
    result += calculate_point_light(
      point_light,
      norm,
      frag_pos,
      view_dir,
      material_ambient, 
      material_diffuse, 
      material_specular
    );
  }

  if(use_spotlight) {
    result += calculate_spotlight(
      spotlight,
      norm,
      frag_pos,
      view_dir,
      material_ambient, 
      material_diffuse, 
      material_specular
    );
  }

  vec3 emission_value = vec3(0.0);
  if(material.light_map.use_emission_map) {
    emission_value = texture(material.light_map.emission_map, tex_coord).rgb * material.light_map.emission_intensity;
    if(length(material_specular) > 0.01 ) {
      emission_value = vec3(0.0);
    }
  }
  result += emission_value;
  frag_color = vec4(result, 1.0);
}