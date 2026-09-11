#include <glad/gl.h>
#include <stdio.h>

#include "stb_image.h"
#include "texture.h"

void loadTexture(GLuint *texture_id, char *texture_path) {
  int width, height, nr_channels;
  unsigned char *data =
      stbi_load(texture_path, &width, &height, &nr_channels, 3);
  glGenTextures(1, texture_id);
  glBindTexture(GL_TEXTURE_2D, *texture_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  if (data == NULL) {
    fprintf(stderr, "ERROR: Failed to load texture");
  } else {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  stbi_image_free(data);
}