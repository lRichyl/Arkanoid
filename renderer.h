#ifndef RENDERER_H
#define RENDERER_H

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "window.h"
#include <vector>

struct RendererInfo{
     static const int QUADS_PER_BATCH = 1000;
     static const int FLOATS_PER_QUAD = 20;
     static const int INDICES_PER_QUAD = 6;
     static const int TOTAL_INDICES =  QUADS_PER_BATCH * INDICES_PER_QUAD;
     static int MAX_TEXTURE_UNITS_PER_BATCH;
};



struct ShaderProgram{
     unsigned int id;
};
struct Batch{
     unsigned int vbo;
     unsigned int vao;
     unsigned int texture_index = 0;
     unsigned int vertices_index = 0;
     int number_of_quads_to_copy = 0;
     int total_indices_to_draw = 0;
     unsigned int registered_textures_ids[120] = {};// This is just a hardcoded random limit 
     ShaderProgram shader_program;
     // float vertex_buffer[16] = {-0.8f, 0.8f,
     //                          -0.8f, 0.0f,
     //                           0.8f, 0.0f,
     //                           0.8f, 0.8f};
     float vertex_buffer[RendererInfo::QUADS_PER_BATCH * RendererInfo::FLOATS_PER_QUAD] = {};
};
struct Renderer{
     unsigned int ibo;
     Window *window;
     // unsigned int index_buffer[6] = {0,1,2,2,3,0};
     glm::mat4 projection;
     //Use this matrix later to move the camera.
     glm::mat4 view;
     unsigned int index_buffer[RendererInfo::TOTAL_INDICES] = {};
     Batch main_batch;
     ShaderProgram default_shader_program;
};



struct Texture{
     unsigned int id;
     int width;
     int height;
     int channels;
     unsigned char *data_buffer;
};

struct Rect{
     float x;
     float y;
     float w;
     float h;
};

Renderer* create_renderer(Window *window);
void render_quad(Renderer *renderer, Rect *position, Texture *texture, Rect *clip_region = NULL);
Texture make_texture(const char *path);
void renderer_draw(Renderer *renderer);
void destroy_renderer(Renderer *renderer);
#endif
