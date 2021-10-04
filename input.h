#ifndef EVENTS_H
#define EVENTS_H

#include "GLFW/glfw3.h"
#include "window.h"

struct Event{
     int key;
     int action;
};
bool IsKeyPressed(Window *window, int key);
void PollKeyboardEvents();
void SetKeyboardCallback(Window *window);
void PrintEvents();
bool GetNextEvent(Event *event);

#endif
