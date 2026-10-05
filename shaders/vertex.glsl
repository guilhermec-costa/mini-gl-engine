#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 in_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 UV;
out vec3 normal;
out vec3 frag_pos;

void main() {
  gl_Position = projection * view * model * vec4(pos, 1.0);
  frag_pos = vec3(model * vec4(pos, 1.0));
  UV = uv;  
  normal = mat3(transpose(inverse(model))) * in_normal;
}
