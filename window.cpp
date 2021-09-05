#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"
#include "window.h"


Window* create_window(int width, int height, char *title){
     Window *window = new Window;
     stbi_set_flip_vertically_on_load(true);
     if(!glfwInit()){
          printf("glfw could not be initialized.");
          exit(0);
     }
     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
     window->w = glfwCreateWindow(width, height, title, NULL, NULL);
     window->width = width;
     window->height = height;

     if(!window->w){
          glfwTerminate();
          printf("Window could not be created.");
          exit(0);
     }
     glfwMakeContextCurrent(window->w);
     // glfwSwapInterval(1);
     if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
     {
         printf("Failed to initialize GLAD");
         exit(0);
     }
     glEnable(GL_BLEND);
     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
     return window;
}
void swap_buffers(Window *window){
     glfwSwapBuffers(window->w);
}

void poll_events(){
     glfwPollEvents();
}
void destroy_window(Window *window){
     delete window;
}
