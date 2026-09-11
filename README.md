# Shader playground
This is playground for testing screen space shaders, akin to shadertoy, but much more barebone.

## Usage
`shaderpg -v <vertex_shader_path> -f <fragment_shader_path> -t <texture_path>` \
You can ignore these arguments, they will default to these values
- Vertex shader: shaders/test.vert
- Fragment shader: shaders/frag.vert
- Texture: textures/texture.jpg

## How to build
Just clone this repository and run make. Right now it has only been tested on Linux.
Dependencies
- clang
- glfw
Other libraries are included within the repository.
