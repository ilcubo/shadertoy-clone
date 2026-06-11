#version 410 core

in vec2 position;

// uniform float u_time;
// uniform vec2 mouse;
uniform vec2 u_resolution;

out vec4 frag_colour;

void main() {
  float r = gl_FragCoord.x / u_resolution.x;
  float g = 0.0;
  float b = gl_FragCoord.y / u_resolution.y;
  frag_colour = vec4(r, g, b, 1.0);
}