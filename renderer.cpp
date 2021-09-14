#include "glad/glad.h"
#include "utilities.h"
#include "renderer.h"
#include "math.h"
#include "stb_image.h"
#include <assert.h>

bool check_if_texture_is_not_registered(Texture texture, Batch *batch);
void bind_texture(int slot, unsigned int id);
static void rebind_registered_texture_ids(Batch *batch);
void create_shader_program(ShaderProgram *program, unsigned int vertex_shader, unsigned int fragment_shader);
void make_vertex_shader(const char* path, unsigned int *vertex_shader);
void make_fragment_shader(const char* path, unsigned int *fragment_shader);

int RendererInfo::MAX_TEXTURE_UNITS_PER_BATCH = 0;

void create_framebuffer_buffers(Renderer *renderer){
     // glUseProgram(renderer->framebuffer_shader_program.id);

     glGenVertexArrays(1, &renderer->framebuffer_vao);
     glBindVertexArray(renderer->framebuffer_vao);

     glGenBuffers(1, &renderer->framebuffer_ibo);
     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->framebuffer_ibo);
     glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(renderer->framebuffer_index_buffer), (void*)renderer->framebuffer_index_buffer, GL_STATIC_DRAW);

     glGenBuffers(1, &renderer->framebuffer_vbo);
     glBindBuffer(GL_ARRAY_BUFFER, renderer->framebuffer_vbo);
     glBufferData(GL_ARRAY_BUFFER, sizeof(renderer->framebuffer_vertex_buffer), (void*)renderer->framebuffer_vertex_buffer, GL_STATIC_DRAW);

     glEnableVertexAttribArray(0);
     glVertexAttribPointer(0, 2, GL_FLOAT, false, 4 * sizeof(float), (void*)0);
     glEnableVertexAttribArray(1);
     glVertexAttribPointer(1, 2, GL_FLOAT, false, 4 * sizeof(float), (void*)( 2 * sizeof(float)));


     glBindVertexArray(0);
}

void change_drawing_resolution(Renderer *renderer, int width, int height){
     renderer->drawing_resolution.x = width;
     renderer->drawing_resolution.y = height;
     glDeleteTextures(1, &renderer->framebuffer_texture);
     glGenTextures(1, &renderer->framebuffer_texture);
     glActiveTexture(GL_TEXTURE0);
     glBindTexture(GL_TEXTURE_2D, renderer->framebuffer_texture);
     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
     glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
     glBindTexture(GL_TEXTURE_2D, 0);

     glBindFramebuffer(GL_FRAMEBUFFER, renderer->fbo);
     glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderer->framebuffer_texture, 0);
     glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

static void initialize_framebuffer(Renderer *renderer){

     glGenFramebuffers(1, &renderer->fbo);
     glBindFramebuffer(GL_FRAMEBUFFER, renderer->fbo);

     glGenTextures(1, &renderer->framebuffer_texture);
     glActiveTexture(GL_TEXTURE0);
     glBindTexture(GL_TEXTURE_2D, renderer->framebuffer_texture);
     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (int)renderer->drawing_resolution.x, (int)renderer->drawing_resolution.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
     glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
     glBindTexture(GL_TEXTURE_2D, 0);


     // glGenTextures(1, &renderer->framebuffer_rbo);
     // glBindTexture(GL_TEXTURE_2D, renderer->framebuffer_rbo);
     // // glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 800, 600);
     // glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, 400, 300, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
     // glBindTexture(GL_TEXTURE_2D, 0);


     glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderer->framebuffer_texture, 0);
     // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, renderer->framebuffer_rbo,0);
     GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
     glDrawBuffers(1, DrawBuffers);

     if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	    printf("ERROR: Framebuffer is not complete!");
     glBindFramebuffer(GL_FRAMEBUFFER, 0);

     glUseProgram(renderer->framebuffer_shader_program.id);
     // int framebuffer_texture = glGetUniformLocation(renderer->framebuffer_shader_program.id, "screenTexture");
     //
     // glBindTexture(GL_TEXTURE_2D, renderer->framebuffer_texture);
     // glUniform1i(framebuffer_texture, 0);
     glUseProgram(0);

}

static void draw_framebuffer(Renderer *renderer){
     glUseProgram(renderer->framebuffer_shader_program.id);
     glBindVertexArray(renderer->framebuffer_vao);
     // glBindBuffer(GL_ARRAY_BUFFER, renderer->framebuffer_vbo);
     //
     // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->framebuffer_ibo);
     // glBindTexture(GL_TEXTURE_2D, renderer->framebuffer_texture);

     int framebuffer_texture = glGetUniformLocation(renderer->framebuffer_shader_program.id, "screenTexture");
     glActiveTexture(GL_TEXTURE0);
     glBindTexture(GL_TEXTURE_2D, renderer->framebuffer_texture);
     glUniform1i(framebuffer_texture, 0);

     glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
     glBindTexture(GL_TEXTURE_2D, 0);
     glBindVertexArray(0);
     glUseProgram(0);
}
static void initialize_renderer_index_buffer(Renderer *renderer){
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
static void initialize_batch_vertex_buffers_and_arrays(Batch *batch, Renderer *renderer){
     batch->shader_program = renderer->default_shader_program;
     glGenVertexArrays(1, &batch->vao);
     glBindVertexArray(batch->vao);
     glGenBuffers(1, &batch->vbo);
     glBindBuffer(GL_ARRAY_BUFFER, batch->vbo);
     glBufferData(GL_ARRAY_BUFFER, sizeof(batch->vertex_buffer), nullptr, GL_DYNAMIC_DRAW);

     glEnableVertexAttribArray(0);
     glVertexAttribPointer(0, 2, GL_FLOAT, false, 6 * sizeof(float), (void*)0);
     glEnableVertexAttribArray(1);
     glVertexAttribPointer(1, 2, GL_FLOAT, false, 6 * sizeof(float), (void*)( 2 * sizeof(float)));
     glEnableVertexAttribArray(2);
     glVertexAttribPointer(2, 1, GL_FLOAT, false, 6 * sizeof(float), (void*)( 4 * sizeof(float)));
     glEnableVertexAttribArray(3);
     glVertexAttribPointer(3, 1, GL_FLOAT, false, 6 * sizeof(float), (void*)( 5 * sizeof(float)));


     glBindVertexArray(0);
}

void initialize_batch_texture_sampler(Batch *batch){
     glUseProgram(batch->shader_program.id);
     int max_texture_units;
     glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_texture_units);
     RendererInfo::MAX_TEXTURE_UNITS_PER_BATCH = max_texture_units;
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
     glUseProgram(0);
}

void load_mvp_to_shader(Renderer *renderer){
     glUseProgram(renderer->main_batch.shader_program.id);
     int projection_uniform_id = glGetUniformLocation(renderer->main_batch.shader_program.id, ("u_projection"));
     glUniformMatrix4fv(projection_uniform_id, 1, GL_FALSE, glm::value_ptr(renderer->projection));

     int view_uniform_id = glGetUniformLocation(renderer->main_batch.shader_program.id, ("u_view"));
     glUniformMatrix4fv(view_uniform_id, 1, GL_FALSE, glm::value_ptr(renderer->view));
     glUseProgram(0);
}

void compile_shader_program(ShaderProgram *shader_program, char *vs_path, char *fs_path){
     unsigned int vs;
     unsigned int fs;
     make_vertex_shader(vs_path, &vs);
     make_fragment_shader(fs_path, &fs);
     create_shader_program(shader_program, vs, fs);
     printf("SHADER PROGRAM\n");
     printf("%s\n", vs_path);
     printf("%s\n", fs_path);
     printf("Succesfully compiled\n\n");
}

void initialize_renderer(Renderer *renderer, Window *window){
     renderer->projection = glm::ortho(0.0f, (float)window->internalWidth, 0.0f, (float)window->internalHeight, -1.0f, 1.0f);
     renderer->view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); //Modify this in real time to move the camera.
     compile_shader_program(&renderer->default_shader_program, "assets/shaders/default_vertex_shader.txt", "assets/shaders/default_fragment_shader.txt");
     compile_shader_program(&renderer->framebuffer_shader_program, "assets/shaders/framebuffer_vertex_shader.txt", "assets/shaders/framebuffer_fragment_shader.txt");
     create_framebuffer_buffers(renderer);
     initialize_framebuffer(renderer);

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
     glDeleteFramebuffers(1, &renderer->fbo);
     glDeleteTextures(1, &renderer->framebuffer_texture);
     delete renderer;
}

void render_quad(Renderer *renderer, Rect *position, Texture *texture, Rect *clip_region, bool mirror, float alphaValue){
     Window *win = renderer->window;
     V2 top_left_clip;
     V2 bottom_left_clip;
     V2 top_right_clip;
     V2 bottom_right_clip;
     float normalizedAlphaValue = alphaValue / 255.f;

     //The clip_region is used to select a part of a texture that we want to render.
     if(clip_region){
          assert(clip_region->w <= texture->width);
          assert(clip_region->h <= texture->height);

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
     if(mirror){
          V2::SwitchXComponents(&top_left_clip, &top_right_clip);
          V2::SwitchXComponents(&bottom_left_clip, &bottom_right_clip);
     }

     //We do this so that if we try to draw outside the window we don't add data to the vertex buffer.
     if((position->x + position->w >= 0) && (position->x <= win->internalWidth) && (position->y >= 0) && (position->y - position->h <= win->internalHeight)){
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
          assert(current_batch->texture_index <= RendererInfo::MAX_TEXTURE_UNITS_PER_BATCH);


          current_batch->vertex_buffer[current_batch->vertices_index] = position->x;
          current_batch->vertex_buffer[current_batch->vertices_index + 1] = position->y;
          current_batch->vertex_buffer[current_batch->vertices_index + 2] = top_left_clip.x;
          current_batch->vertex_buffer[current_batch->vertices_index + 3] = top_left_clip.y;
          current_batch->vertex_buffer[current_batch->vertices_index + 4] = texture_slot_id;
          current_batch->vertex_buffer[current_batch->vertices_index + 5] = normalizedAlphaValue;

          current_batch->vertex_buffer[current_batch->vertices_index + 6] =  position->x;
          current_batch->vertex_buffer[current_batch->vertices_index + 7] = position->y - position->h;
          current_batch->vertex_buffer[current_batch->vertices_index + 8] = bottom_left_clip.x;
          current_batch->vertex_buffer[current_batch->vertices_index + 9] = bottom_left_clip.y;
          current_batch->vertex_buffer[current_batch->vertices_index + 10] = texture_slot_id;
          current_batch->vertex_buffer[current_batch->vertices_index + 11] = normalizedAlphaValue;

          current_batch->vertex_buffer[current_batch->vertices_index + 12] = position->x + position->w;
          current_batch->vertex_buffer[current_batch->vertices_index + 13] = position->y - position->h;
          current_batch->vertex_buffer[current_batch->vertices_index + 14] = bottom_right_clip.x;
          current_batch->vertex_buffer[current_batch->vertices_index + 15] = bottom_right_clip.y;
          current_batch->vertex_buffer[current_batch->vertices_index + 16] = texture_slot_id;
          current_batch->vertex_buffer[current_batch->vertices_index + 17] = normalizedAlphaValue;

          current_batch->vertex_buffer[current_batch->vertices_index + 18] = position->x + position->w;
          current_batch->vertex_buffer[current_batch->vertices_index + 19] = position->y;
          current_batch->vertex_buffer[current_batch->vertices_index + 20] = top_right_clip.x;
          current_batch->vertex_buffer[current_batch->vertices_index + 21] = top_right_clip.y;
          current_batch->vertex_buffer[current_batch->vertices_index + 22] = texture_slot_id;
          current_batch->vertex_buffer[current_batch->vertices_index + 23] = normalizedAlphaValue;


          current_batch->vertices_index += RendererInfo::FLOATS_PER_QUAD;
          current_batch->number_of_quads_to_copy++;
          current_batch->total_indices_to_draw += RendererInfo::INDICES_PER_QUAD;

          assert(current_batch->number_of_quads_to_copy <= RendererInfo::QUADS_PER_BATCH);

     }
}

void renderer_draw(Renderer *renderer){
     Batch *current_batch = &renderer->main_batch;

     //We need to rebind the registered textures in each batch before drawing to the framebuffer.
     //If we don't the batch´s textures get overwritten by the framebuffer texture. And we don't want
     //to reserve a texture unit just for the framebuffer.
     //What happened was that we used the texture unit 0 for the frambuffer texture, so the first texture
     //of the batch would get overwritten.
     //This will have to be done for every batch when we implement multiple batches.
     rebind_registered_texture_ids(current_batch);

     glViewport(0,0,(int)renderer->drawing_resolution.x, (int)renderer->drawing_resolution.y);
     glUseProgram(current_batch->shader_program.id);
     glBindFramebuffer(GL_FRAMEBUFFER, renderer->fbo);
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     glEnable(GL_DEPTH_TEST);
     glBindVertexArray(current_batch->vao);
     glBindBuffer(GL_ARRAY_BUFFER, current_batch->vbo);

     int bytes_to_copy = current_batch->vertices_index * sizeof(float);
     //We only copy the vertex data that we are going to draw instead of copying the entire preallocated buffer.
     //This way we can preallocate a vertex buffer of any size and not affect performance.
     glBufferSubData(GL_ARRAY_BUFFER, 0, bytes_to_copy, (void*)current_batch->vertex_buffer);
     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->ibo);
     glDrawElements(GL_TRIANGLES, current_batch->total_indices_to_draw, GL_UNSIGNED_INT, 0);
     glBindVertexArray(0);
     glBindFramebuffer(GL_FRAMEBUFFER, 0);


     // glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     glDisable(GL_DEPTH_TEST);



     // glUseProgram(renderer->framebuffer_shader_program.id);
     // int framebuffer_texture = glGetUniformLocation(renderer->framebuffer_shader_program.id, "screenTexture");
     // glActiveTexture(GL_TEXTURE0 + current_batch->texture_index);
     // glBindTexture(GL_TEXTURE_2D, renderer->framebuffer_texture);
     // glUniform1i(framebuffer_texture, 0);

     current_batch->vertices_index = 0;
     current_batch->number_of_quads_to_copy = 0;
     current_batch->total_indices_to_draw = 0;
     // current_batch->texture_index = 0;
     int width, height;
     glfwGetWindowSize(renderer->window->GLFWInstance, &width, &height);

     glViewport(0, 0, width, height);
     draw_framebuffer(renderer);


     // glDeleteTextures(1, &renderer->framebuffer_texture);
     // glDeleteFramebuffers(1, &renderer->fbo);
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
     glDeleteShader(vertex_shader);
     glDeleteShader(fragment_shader);
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
          // printf("Vertex shader succesfully compiled \n");
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
          // printf("Fragment shader succesfully compiled \n");
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
     // printf("Texture ID: %d\n", texture.id);
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

static void rebind_registered_texture_ids(Batch *batch){
     for(int i = 0; i < RendererInfo::MAX_TEXTURE_UNITS_PER_BATCH; i++){
          bind_texture( i, batch->registered_textures_ids[i]);
     }
}
