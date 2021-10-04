
#include <stdio.h>
#include <windows.h>
#include "time.h"
#include "renderer.h"
#include "game.h"
#include "text.h"
#include "input.h"
#include <cmath>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_truetype.h"


void WindowResizeCallback(GLFWwindow* window, int width, int height){
     glViewport(0, 0, width, height);
}

int main(){
     Window *window;
     //The size here is the size of the window and the initial drawing resolution.
     //The internal resolution can be set manually and it's the one used for the game calculations.
     window = create_window(800, 600, "Game Engine");

     printf("%s \n",glGetString(GL_VERSION));
     Renderer *renderer = create_renderer(window);

     if(!renderer){
          printf("Error creating the renderer\n");
          exit(0);
     }

     float fixed_fps = 60.0f;
     float dt_in_ms = 1.0f / fixed_fps * 1000.0f;
     float dt = 0;
     const int NUM_SAMPLES = 100;
     int sample_count = 0;
     float samples[NUM_SAMPLES] = {};

     //printf("%f", dt);

     long long perf_count_frequency = get_performance_counter_frequency();
     LARGE_INTEGER start_time = get_time_counter();

     Game game = Game(renderer, window);
     // bool showFPS = true;
     // Texture font = make_texture("assets/fonts/font.png");
     // Text FPSText = Text(font, 12, V2{0,float(window->internalHeight)});
     glfwSetWindowSizeCallback(window->GLFWInstance,WindowResizeCallback);
     SetKeyboardCallback(window);
     Font test = Font("assets/fonts/Simvoni.ttf", 20);

     while(!glfwWindowShouldClose(window->GLFWInstance)){


          //This is done because when we select the window it stops the execution of
          //the program so the next time we get the current time and calculate dt
          //it will be so big that physics break.
          if(dt > 0 && dt < 0.1){
               game.GameLoop(dt);

          }
          Timer::dt = dt;

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
          float realfps;
          // std::string fps_string;
          // char *c;
          if(game.showFPS && sample_count == NUM_SAMPLES - 1){
               float sum = 0;
               for(int i = 0; i < sample_count; i++){
                    sum += samples[i];
               }
               float average_ms = sum / (NUM_SAMPLES);
               int average_fps = 1.f / (average_ms / 1000.f);
               sample_count = 0;

               // printf("Real FPS: %f ", 1.f / (ms_per_frame / 1000.f));
               // printf("%f ms , %i FPS\n",waited_time + ms_per_frame, fps);
               realfps =  1.f / (ms_per_frame / 1000.f);

          }
          std::string fps_string = "FPS: "  + std::to_string((int)realfps);
          char *c_string = &fps_string[0];
          // render_text(renderer, &test, c_string, V2 {0 , 599});

          if(game.showFPS){
               samples[sample_count] = ms_per_frame;
               sample_count++;
          }
          // FPSText.Render(renderer, "Pepe pecas pica papas con un pico");

          // Sleep(1);
     }
     glfwTerminate();
     destroy_window(window);
     destroy_renderer(renderer);
     return 0;
}
