#version 410 core

in vec3 vertex_position;

out vec2 position;

void main() {
  position = vec2(vertex_position.x, vertex_position.y);
  gl_Position = vec4(vertex_position, 1.0);
}