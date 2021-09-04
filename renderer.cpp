#include "glad/glad.h"
#include "utilities.h"
#include "renderer.h"
#include "math.h"
#include "stb_image.h"
#include <assert.h>

bool check_if_texture_is_not_registered(Texture texture, Batch *batch);
void bind_texture(int slot, unsigned int id);
void create_shader_program(ShaderProgram *program, unsigned int vertex_shader, unsigned int fragment_shader);
void make_vertex_shader(const char* path, unsigned int *vertex_shader);
void make_fragment_shader(const char* path, unsigned int *fragment_shader);

int RendererInfo::MAX_TEXTURE_UNITS_PER_BATCH = 0;

void initialize_renderer_index_buffer(Renderer *renderer){
     int indices_index = 0;
     for(int i = 0; i< RendererInfo::TOTAL_INDICES ;) {
          for(int j = 0; j< 6 ;j++, i++) {
                    if(i < 6) {
                         renderer->index_buffer[0] = 0;
                         renderer->index_buffer[1] = 1;
                         renderer->index_buffer[2] = 2;
                         renderer->index_buffer[3] = 2;
                         renderer->index_buffer[4] = 3;
                         renderer->index_buffer[5] = 0;
                        indices_index = 6;
                        i = 6;
                        break;
                    }else {
                         renderer->index_buffer[i] = renderer->index_buffer[i - 6] + 4;
                    }
                    //printf("%d ", renderer->index_buffer[i]);
          }
          //int x = test_int;
          //printf("\n");
          glGenBuffers(1, &renderer->ibo);
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->ibo);
          glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(renderer->index_buffer), (void*)renderer->index_buffer, GL_DYNAMIC_DRAW);
     }
}
void initialize_batch_vertex_buffers_and_arrays(Batch *batch, Renderer *renderer){
     batch->shader_program = renderer->default_shader_program;
     glGenVertexArrays(1, &batch->vao);
     glBindVertexArray(batch->vao);
     glGenBuffers(1, &batch->vbo);
     glBindBuffer(GL_ARRAY_BUFFER, batch->vbo);
     glBufferData(GL_ARRAY_BUFFER, sizeof(batch->vertex_buffer), nullptr, GL_DYNAMIC_DRAW);

     glEnableVertexAttribArray(0);
     glVertexAttribPointer(0, 2, GL_FLOAT, false, 5 * sizeof(float), (void*)0);
     glEnableVertexAttribArray(1);
     glVertexAttribPointer(1, 2, GL_FLOAT, false, 5 * sizeof(float), (void*)( 2 * sizeof(float)));
     glEnableVertexAttribArray(2);
     glVertexAttribPointer(2, 1, GL_FLOAT, false, 5 * sizeof(float), (void*)( 4 * sizeof(float)));


     glBindVertexArray(0);
}

void initialize_batch_texture_sampler(Batch *batch){
     glUseProgram(batch->shader_program.id);
     int max_texture_units;
     RendererInfo::MAX_TEXTURE_UNITS_PER_BATCH = max_texture_units;
     glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_texture_units);
     int max_texture_units_id = glGetUniformLocation(batch->shader_program.id, "u_max_texture_units");
     glUniform1i(max_texture_units_id, max_texture_units);
     // printf("%d", max_texture_units);

     int texture_sampler_id = glGetUniformLocation(batch->shader_program.id, "u_textures");
     int *a = new int[max_texture_units];
     for(int i = 0; i < max_texture_units; i++){
          a[i] = i;
     }
     glUniform1iv(texture_sampler_id, max_texture_units, a);
     delete a;
}

void load_mvp_to_shader(Renderer *renderer){
     glUseProgram(renderer->main_batch.shader_program.id);
     int projection_uniform_id = glGetUniformLocation(renderer->main_batch.shader_program.id, ("u_projection"));
     glUniformMatrix4fv(projection_uniform_id, 1, GL_FALSE, glm::value_ptr(renderer->projection));

     int view_uniform_id = glGetUniformLocation(renderer->main_batch.shader_program.id, ("u_view"));
     glUniformMatrix4fv(view_uniform_id, 1, GL_FALSE, glm::value_ptr(renderer->view));
}

void compile_default_shader_program(Renderer *renderer){
     unsigned int vs;
     unsigned int fs;
     make_vertex_shader("assets/shaders/default_vertex_shader.txt", &vs);
     make_fragment_shader("assets/shaders/default_fragment_shader.txt", &fs);
     create_shader_program(&renderer->default_shader_program, vs, fs);
}

void initialize_renderer(Renderer *renderer, Window *window){
     renderer->projection = glm::ortho(0.0f, (float)window->width, 0.0f, (float)window->height, -1.0f, 1.0f);
     renderer->view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
     compile_default_shader_program(renderer);
     initialize_renderer_index_buffer(renderer);

     //This should be in a loop when we add multiple batches to initialize them.
     initialize_batch_vertex_buffers_and_arrays(&renderer->main_batch, renderer);
     initialize_batch_texture_sampler(&renderer->main_batch);
     //This needs to happen after the shader program is compiled.
     load_mvp_to_shader(renderer);

}

Renderer* create_renderer(Window *window){
     Renderer *renderer = new Renderer;
     initialize_renderer(renderer, window);
     renderer->window = window;
     // printf("%d", renderer->window->width);
     return renderer;
}

void destroy_renderer(Renderer *renderer){
     delete renderer;
}

void render_quad(Renderer *renderer, Rect *position, Texture *texture, Rect *clip_region){
     Window *win = renderer->window;
     v2 top_left_clip;
     v2 bottom_left_clip;
     v2 top_right_clip;
     v2 bottom_right_clip;

     if(clip_region){
          assert(clip_region->w < texture->width);
          assert(clip_region->h < texture->height);

          top_left_clip.x     = clip_region->x / texture->width;
          top_left_clip.y     = (clip_region->y + clip_region->h) / texture->height;
          bottom_left_clip.x  = clip_region->x / texture->width;
          bottom_left_clip.y  = clip_region->y / texture->height;
          bottom_right_clip.x = (clip_region->x + clip_region->w) / texture->width;
          bottom_right_clip.y = clip_region->y / texture->height;
          top_right_clip.x    = (clip_region->x + clip_region->w) / texture->width;
          top_right_clip.y    = (clip_region->y + clip_region->h) / texture->height;
     }else{
          top_left_clip.x     = 0.0f;
          top_left_clip.y     = 1.0f;
          bottom_left_clip.x  = 0.0f;
          bottom_left_clip.y  = 0.0f;
          bottom_right_clip.x = 1.0f;
          bottom_right_clip.y = 0.0f;
          top_right_clip.x    = 1.0f;
          top_right_clip.y    = 1.0f;
     }

     //We do this so that if we try to draw outside the window we don't add data to the vertex bufer.
     if((position->x + position->w >= 0) && (position->x <= win->width) && (position->y >= 0) && (position->y - position->h <= win->height)){
          Batch *current_batch = &renderer->main_batch;
          float texture_slot_id = 0;
          if(check_if_texture_is_not_registered(*texture, current_batch)){
               current_batch->registered_textures_ids[current_batch->texture_index] = texture->id;
               bind_texture(current_batch->texture_index, texture->id);
               texture_slot_id = (float)current_batch->texture_index;
               current_batch->texture_index++;
          }else{
               for(int i = 0; i < RendererInfo::MAX_TEXTURE_UNITS_PER_BATCH; i++){
                    if(texture->id == current_batch->registered_textures_ids[i]){
                         texture_slot_id = i;
                         break;
                    }
               }
          }

          assert(current_batch->texture_index + 1 < RendererInfo::MAX_TEXTURE_UNITS_PER_BATCH);
          // printf("%d", current_batch->texture_index);

          current_batch->vertex_buffer[current_batch->vertices_index] = position->x;
          current_batch->vertex_buffer[current_batch->vertices_index + 1] = position->y;
          current_batch->vertex_buffer[current_batch->vertices_index + 2] = top_left_clip.x;
          current_batch->vertex_buffer[current_batch->vertices_index + 3] = top_left_clip.y;
          current_batch->vertex_buffer[current_batch->vertices_index + 4] = texture_slot_id;

          current_batch->vertex_buffer[current_batch->vertices_index + 5] =  position->x;
          current_batch->vertex_buffer[current_batch->vertices_index + 6] = position->y - position->h;
          current_batch->vertex_buffer[current_batch->vertices_index + 7] = bottom_left_clip.x;
          current_batch->vertex_buffer[current_batch->vertices_index + 8] = bottom_left_clip.y;
          current_batch->vertex_buffer[current_batch->vertices_index + 9] = texture_slot_id;

          current_batch->vertex_buffer[current_batch->vertices_index + 10] = position->x + position->w;
          current_batch->vertex_buffer[current_batch->vertices_index + 11] = position->y - position->h;
          current_batch->vertex_buffer[current_batch->vertices_index + 12] = bottom_right_clip.x;
          current_batch->vertex_buffer[current_batch->vertices_index + 13] = bottom_right_clip.y;
          current_batch->vertex_buffer[current_batch->vertices_index + 14] = texture_slot_id;

          current_batch->vertex_buffer[current_batch->vertices_index + 15] = position->x + position->w;
          current_batch->vertex_buffer[current_batch->vertices_index + 16] = position->y;
          current_batch->vertex_buffer[current_batch->vertices_index + 17] = top_right_clip.x;
          current_batch->vertex_buffer[current_batch->vertices_index + 18] = top_right_clip.y;
          current_batch->vertex_buffer[current_batch->vertices_index + 19] = texture_slot_id;

          // int i = 0;
          // int j = 0;
          // for(i = 0; i < 4; i++){
               //      printf("[%d]", i);
               //      for(j = 0; j < 5; j++){
                    //           printf("%f, ", current_batch->vertex_buffer[current_batch->vertices_index + (j + (i * 5))]);
                    //      }
                    //      printf("\n");
                    // }
                    current_batch->vertices_index += 20;
                    current_batch->number_of_quads_to_copy++;
                    current_batch->total_indices_to_draw += 6;

     }
}

void renderer_draw(Renderer *renderer){
     Batch *current_batch = &renderer->main_batch;
     glUseProgram(current_batch->shader_program.id);
     glBindVertexArray(current_batch->vao);
     glBindBuffer(GL_ARRAY_BUFFER, current_batch->vbo);

     int bytes_to_copy = current_batch->vertices_index * sizeof(float);
     //We only copy the vertex data that we are going to draw instead of copying the entire preallocated buffer.
     //This way we can preallocate a vertex buffer of any size and not affect performance.
     glBufferSubData(GL_ARRAY_BUFFER, 0, bytes_to_copy, (void*)current_batch->vertex_buffer);
     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->ibo);
     glDrawElements(GL_TRIANGLES, current_batch->total_indices_to_draw, GL_UNSIGNED_INT, 0);
     glBindVertexArray(0);

     current_batch->vertices_index = 0;
     current_batch->number_of_quads_to_copy = 0;
     current_batch->total_indices_to_draw = 0;
}

void create_shader_program(ShaderProgram *program, unsigned int vertex_shader, unsigned int fragment_shader){
     program->id = glCreateProgram();
     unsigned int id = program->id;
     glAttachShader(id, vertex_shader);
     glAttachShader(id, fragment_shader);
     glLinkProgram(id);
     int success;
     glGetProgramiv(id, GL_LINK_STATUS, &success);
     if( success == GL_FALSE){
          printf("Could not link the shader program \n");
          exit(1);
     }

     glDetachShader(id, vertex_shader);
     glDetachShader(id, fragment_shader);
}

void make_vertex_shader(const char* path, unsigned int *vertex_shader){
     char *source = text_file_to_char(path);
     *vertex_shader = glCreateShader(GL_VERTEX_SHADER);
     glShaderSource(*vertex_shader, 1 , &source, NULL);
     glCompileShader(*vertex_shader);

     int  success;
     char infoLog[512];
     glGetShaderiv(*vertex_shader, GL_COMPILE_STATUS, &success);

     if(!success)
     {
         glGetShaderInfoLog(*vertex_shader, 512, NULL, infoLog);
         printf("Vertex shader compilation failed: \n %s", infoLog);
     }
     else{
          printf("Vertex shader succesfully compiled \n");
     }
     free(source);
}

void make_fragment_shader(const char* path, unsigned int *fragment_shader){
     char *source = text_file_to_char(path);
     *fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
     glShaderSource(*fragment_shader, 1 , &source, NULL);
     glCompileShader(*fragment_shader);

     int  success;
     char infoLog[512];
     glGetShaderiv(*fragment_shader, GL_COMPILE_STATUS, &success);

     if(!success)
     {
         glGetShaderInfoLog(*fragment_shader, 512, NULL, infoLog);
         printf("Fragmetn shader compilation failed: \n %s", infoLog);
     }
     else{
          printf("Fragment shader succesfully compiled \n");
     }
     free(source);
}

Texture make_texture(const char *path){
     Texture texture;
     texture.data_buffer = stbi_load(path, &texture.width, &texture.height, &texture.channels, 0);
     if(!texture.data_buffer){
          printf("Could not find a texture image at the relative path: %s\n", path);
          exit(-1);
     }
     glGenTextures(1, &texture.id);
     glBindTexture(GL_TEXTURE_2D, texture.id);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)texture.data_buffer);
     glBindTexture(GL_TEXTURE_2D, 0);
     stbi_image_free(texture.data_buffer);
     return texture;
}

void bind_texture(int slot, unsigned int id){
     glActiveTexture(GL_TEXTURE0 + slot);
     glBindTexture(GL_TEXTURE_2D, id);
}

bool check_if_texture_is_not_registered(Texture texture, Batch *batch){
     int difference_count = 0;
     // static int count = 0;
     for(int i = 0; i < RendererInfo::MAX_TEXTURE_UNITS_PER_BATCH; i++){
          if(texture.id != batch->registered_textures_ids[i]){
               difference_count++;
          }
     }
     if(difference_count == RendererInfo::MAX_TEXTURE_UNITS_PER_BATCH){
          // count++;
          // printf("%d ", count );
          //
          // if(count >= 2){
          //      count = 0;
          //      printf("\n");
          // }
          return true;
     }else{
          // printf("%d ", count );
          return false;
     }
}
