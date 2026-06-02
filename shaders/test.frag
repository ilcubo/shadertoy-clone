#version 410 core

in vec2 position;

uniform float u_time;
// uniform vec2 mouse;
uniform vec2 u_resolution;

out vec4 frag_colour;

void main() {
  float r = gl_FragCoord.x / u_resolution.x;
  float g = 1.0;
  float b = 1.0;
  frag_colour = vec4(r, sin(u_time), 0.0, 1.0);
}