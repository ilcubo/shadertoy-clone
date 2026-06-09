#include <glad/gl.h>

#include <GLFW/glfw3.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

void loadFile(char **dest, char *file_path) {
  FILE *fptr = fopen(file_path, "r");
  fseek(fptr, 0, SEEK_END);

  long size = ftell(fptr);
  *dest = (char *)malloc(size + 1);

  fseek(fptr, 0, SEEK_SET);
  fread(*dest, 1, size, fptr);
  (*dest)[size] = '\0';
  fclose(fptr);
}

void compileShader(GLuint *shader_program, char *vert_src, char *frag_src) {
  int success;
  char info_log[512];

  GLuint vert_shader;
  vert_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vert_shader, 1, &vert_src, NULL);
  glCompileShader(vert_shader);
  glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vert_shader, 512, NULL, info_log);
    fprintf(stderr, "ERROR[opengl]: Vertex shader compilation failed\n%s\n",
            info_log);
  }

  GLuint frag_shader;
  frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag_shader, 1, &frag_src, NULL);
  glCompileShader(frag_shader);
  glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(frag_shader, 512, NULL, info_log);
    fprintf(stderr, "ERROR[opengl]: Fragment shader compilation failed\n%s\n",
            info_log);
  }

  *shader_program = glCreateProgram();
  glAttachShader(*shader_program, vert_shader);
  glAttachShader(*shader_program, frag_shader);
  glLinkProgram(*shader_program);
  glGetProgramiv(*shader_program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(*shader_program, 512, NULL, info_log);
    fprintf(stderr, "ERROR[opengl]: Shader program linking failed\n%s\n",
            info_log);
  }

  glDeleteShader(vert_shader);
  glDeleteShader(frag_shader);
}

void loadShaderFromFile(GLuint *shader_program, char *vert_path,
                        char *frag_path) {
  char *vert_src;
  char *frag_src;

  loadFile(&vert_src, vert_path);
  loadFile(&frag_src, frag_path);
  compileShader(shader_program, vert_src, frag_src);
}

int main(int argc, char *argv[]) {
  char *vert_path = "shaders/test.vert";
  char *frag_path = "shaders/test.frag";

  char opt;
  while ((opt = getopt(argc, argv, "v:f:")) != -1) {
    switch (opt) {
    case 'v':
      printf("-v: %s\n", optarg);
      break;
    case 'f':
      printf("-f: %s\n", optarg);
      break;
    default:
      fprintf(stderr, "Invalid option!. Use -h to see all available options.");
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
  GLint resolution_loc = glGetUniformLocation(shader_program, "u_resolution");

  while (!glfwWindowShouldClose(window)) {
    double curr_s = glfwGetTime();

    int frame_w;
    int frame_h;
    glfwGetFramebufferSize(window, &frame_w, &frame_h);

    glfwPollEvents();

    double mouse_x;
    double mouse_y;
    glfwGetCursorPos(window, &mouse_x, &mouse_y);

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
      resolution_loc = glGetUniformLocation(shader_program, "u_resolution");
    }
  }

  glfwPollEvents();
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
