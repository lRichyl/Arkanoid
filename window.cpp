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
     window->GLFWInstance = glfwCreateWindow(width, height, title, NULL, NULL);

     //This variables represent the internal resolution. Every calculation is based on these so that
     //if you resize the window the everything scales correctly.
     //This should be set only once at the start of the program.
     window->internalWidth = 800;
     window->internalHeight = 600;

     if(!window->GLFWInstance){
          glfwTerminate();
          printf("Window could not be created.");
          exit(0);
     }
     glfwMakeContextCurrent(window->GLFWInstance);
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
     glfwSwapBuffers(window->GLFWInstance);
}

void poll_events(){
     glfwPollEvents();
}
void destroy_window(Window *window){
     delete window;
}
