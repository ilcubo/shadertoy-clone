#include <glad/gl.h>

#include <GLFW/glfw3.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "shader.h"

float vertices[] = {
    1.0f,  1.0f,  0.0f, // Top-Right
    1.0f,  -1.0f, 0.0f, // Bottom-Right
    -1.0f, -1.0f, 0.0f, // Bottom-Left
    -1.0f, 1.0f,  0.0f, // Top-Left
};

GLuint indices[] = {
    0, 1, 3, // First triangle
    1, 2, 3  // Second triangle
};

void errorCallback(int code, const char *description) {
  fprintf(stderr, "ERROR[%d]: %s", code, description);
}

int main(int argc, char *argv[]) {
  char vert_path[512] = "shaders/test.vert";
  char frag_path[512] = "shaders/test.frag";

  char opt;
  while ((opt = getopt(argc, argv, "v:f:")) != -1) {
    switch (opt) {
    case 'v':
      printf("-v: %s\n", optarg);
      strncpy(vert_path, optarg, 512);
      break;
    case 'f':
      printf("-f: %s\n", optarg);
      strncpy(frag_path, optarg, 512);
      break;
    default:
      printf("Usage: shaderpg [-v <PATHNAME>] [-f <PATHNAME>]\n\n-v: Specify a "
             "vertex shader source. Defaults to \"shaders/test.frag\".\n-f: "
             "Specify "
             "a fragment shader source. Defaults to \"shaders/test.vert\".\n"
             "Paths must be less than 512 characters long.");
      exit(1);
    }
  }

  if (!glfwInit()) {
    return 1;
  }

  GLFWwindow *window;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_SCALE_FRAMEBUFFER, GL_TRUE);

  window = glfwCreateWindow(1280, 720, "Shader Playground", NULL, NULL);

  if (!window) {
    return 1;
  }

  glfwMakeContextCurrent(window);

  int version_glad = gladLoadGL(glfwGetProcAddress);
  if (version_glad == 0) {
    fprintf(stderr, "ERROR[glad]: Failed to initialize OpenGL Context.\n");
  }

  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices, GL_STATIC_DRAW);

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  GLuint ebo;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), indices,
               GL_STATIC_DRAW);

  GLuint shader_program;
  loadShaderFromFile(&shader_program, vert_path, frag_path);

  // Get uniform locations
  GLint time_loc = glGetUniformLocation(shader_program, "u_time");
  GLint mouse_loc = glGetUniformLocation(shader_program, "u_mouse");
  GLint mouse_pressed_loc =
      glGetUniformLocation(shader_program, "u_mouse_pressed");
  GLint resolution_loc = glGetUniformLocation(shader_program, "u_resolution");

  while (!glfwWindowShouldClose(window)) {
    double curr_s = glfwGetTime();

    int frame_w;
    int frame_h;
    glfwGetFramebufferSize(window, &frame_w, &frame_h);

    glfwPollEvents();

    float xscale, yscale;
    glfwGetWindowContentScale(window, &xscale, &yscale);
    printf("%f, %f\n", xscale, yscale);

    double mouse_x;
    double mouse_y;
    glfwGetCursorPos(window, &mouse_x, &mouse_y);
    mouse_x *= xscale;
    mouse_y *= yscale;

    int lmb_state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    int mouse_pressed_val = lmb_state == GLFW_PRESS ? 1 : 0;

    glViewport(0, 0, frame_w, frame_h);
    glClearColor(0.175, 0.175, 0.175, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnableVertexAttribArray(0);
    glUseProgram(shader_program);
    // Use uniforms
    if (time_loc > -1) {
      glUniform1f(time_loc, (float)curr_s);
    }
    if (mouse_loc > -1) {
      glUniform2f(mouse_loc, (float)mouse_x, (float)mouse_y);
    }
    if (mouse_pressed_loc > -1) {
      glUniform1i(mouse_pressed_loc, mouse_pressed_val);
    }
    if (resolution_loc > -1) {
      glUniform2f(resolution_loc, (float)frame_w, (float)frame_h);
    }
    // Draw
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glfwSwapBuffers(window);

    if (glfwGetKey(window, GLFW_KEY_R)) {
      loadShaderFromFile(&shader_program, vert_path, frag_path);
      // Get uniform locations
      time_loc = glGetUniformLocation(shader_program, "u_time");
      mouse_loc = glGetUniformLocation(shader_program, "u_mouse");
      mouse_pressed_loc =
          glGetUniformLocation(shader_program, "u_mouse_pressed");
      resolution_loc = glGetUniformLocation(shader_program, "u_resolution");
    }
  }

  glfwPollEvents();
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
