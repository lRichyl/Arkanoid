#ifndef WINDOW_H
#define WINDOW_H
#include "GLFW/glfw3.h"
struct Window{
     GLFWwindow *w;
     int height = 0;
     int width = 0;
};

Window* create_window(int width, int height, char *title);
void swap_buffers(Window *window);
void poll_events();
void destroy_window(Window *window);

#endif
