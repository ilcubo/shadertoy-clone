#include "shader.h"
#include <stdio.h>
#include <stdlib.h>

void loadFile(char **dest, char *file_path) {
  FILE *fptr = fopen(file_path, "r");

  if (fptr == NULL) {
    fprintf(stderr, "ERROR: Can't open the file at path %s", file_path);
    exit(2);
  }

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
