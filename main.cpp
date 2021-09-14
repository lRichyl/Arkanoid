
#include <stdio.h>
#include <windows.h>
#include "renderer.h"
#include "timer.h"
#include "game.h"
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


bool doGameLoop = true;
void WindowResizeCallback(GLFWwindow* window, int width, int height){
     glViewport(0, 0, width, height);
}

int main(){
     Window *window;
     window = create_window(800, 600, "Game Engine");

     printf("%s \n",glGetString(GL_VERSION));
     glViewport(0,0,window->internalWidth,window->internalHeight);

     Renderer *renderer = create_renderer(window);
     if(!renderer){
          printf("Error creating the renderer\n");
          exit(0);
     }

     float fixed_fps = 60.0f;
     float dt_in_ms = 1.0f / fixed_fps * 1000.0f;
     float dt = 0;
     //printf("%f", dt);

     long long perf_count_frequency = get_performance_counter_frequency();
     LARGE_INTEGER start_time = get_time_counter();

     Game game = Game(renderer, window);
     bool showFPS = true;
     glfwSetWindowSizeCallback(window->GLFWInstance,WindowResizeCallback);
     while(!glfwWindowShouldClose(window->GLFWInstance)){

          // if(dt > 0.017){
          //      printf("%f", dt);
          //      continue;
          // }
          //This is done because when we select the window it stops the execution of
          //the program so the next time we get the current time and calculate dt
          //it will be so big that physics break.
          //The correct way to do this would be to poll the events on another thread because
          //windows doesn't get out of the message loop until you stop grabbing the window.
          if(dt > 0 && dt < 0.1){
               game.GameLoop(dt);

          }

          LARGE_INTEGER end_time = get_time_counter();
          float ms_per_frame = get_time_in_ms(start_time, end_time, perf_count_frequency);
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
          if(showFPS){
               printf("Real FPS: %f ", 1.f/(ms_per_frame/1000.f));
               printf("%f ms , %i FPS\n",waited_time + ms_per_frame, fps);
          }
          Sleep(1);




          // doGameLoop = true;
     }
     glfwTerminate();
     destroy_window(window);
     destroy_renderer(renderer);
     return 0;
}
