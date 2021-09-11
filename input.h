#ifndef EVENTS_H
#define EVENTS_H

#include "GLFW/glfw3.h"

bool isKeyPressed(Window *window, int key){
     int state = glfwGetKey(window->GLFWInstance, key);
     if (state == GLFW_PRESS)
     {
         return true;
     }
     return false;
}

#endif
