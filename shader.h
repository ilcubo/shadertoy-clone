#ifndef __SHADER_H__
#define __SHADER_H__
#include <glad/gl.h>
void loadFile(char **dest, char *file_path);
void compileShader(GLuint *shader_program, char *vert_src, char *frag_src);
void loadShaderFromFile(GLuint *shader_program, char *vert_path,
                        char *frag_path);
#endif
