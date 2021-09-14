#include "input.h"


bool isKeyPressed(Window *window, int key){
     int state = glfwGetKey(window->GLFWInstance, key);
     if (state == GLFW_PRESS)
     {
         return true;
     }
     return false;
}
