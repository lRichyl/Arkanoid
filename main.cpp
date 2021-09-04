#include "glad/glad.h"
#include <stdio.h>
#include <windows.h>
// #include "utilities.h"
#include "window.h"
#include "renderer.h"
#include "timer.h"
#include "game.h"
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main(){
     Window *window;
     window = create_window(800, 600, "Game Engine");

     printf("%s \n",glGetString(GL_VERSION));
     glViewport(0,0,window->width,window->height);

     Renderer *renderer = create_renderer(window);
     if(!renderer){
          printf("Error creating the renderer\n");
          exit(0);
     }

     float fixed_fps = 60.0f;
     float dt_in_ms = 1.0f / fixed_fps * 1000.0f;
     float dt;
     //printf("%f", dt);

     long long perf_count_frequency = get_performance_counter_frequency();
     LARGE_INTEGER start_time = get_time_counter();

     Game game;

     while(!glfwWindowShouldClose(window->w)){
          glClear(GL_COLOR_BUFFER_BIT);

          game.GameLoop(dt, renderer, window);

          LARGE_INTEGER end_time = get_time_counter();
          float ms_per_frame = get_time_in_ms(start_time, end_time, perf_count_frequency);
          printf("Real FPS: %f ", 1.f/(ms_per_frame/1000.f));
          start_time = end_time;

          float time_to_wait_for_next_frame = dt_in_ms - ms_per_frame;
          float waited_time = 0;
          LARGE_INTEGER start_busy_wait_time = get_time_counter();
          if(ms_per_frame < dt_in_ms){
               //This is a busy wait loop used to consume the extra time per frame. For a 16.6667 ms goal.
               while(true){
                    LARGE_INTEGER end_busy_wait_time = get_time_counter();
                    float ms = get_time_in_ms(start_busy_wait_time, end_busy_wait_time, perf_count_frequency);
                    waited_time += ms;
                    if(waited_time >= time_to_wait_for_next_frame){
                         break;
                    }
               }
          }
          dt = ms_per_frame / 1000.0f;
          int fps = round(1.0f/((waited_time + ms_per_frame)/1000.0f));
          printf("%f ms , %i FPS\n",waited_time + ms_per_frame, fps);
     }
     glfwTerminate();
     destroy_window(window);
     destroy_renderer(renderer);
     return 0;
}
