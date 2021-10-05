#include "input.h"
#include <queue>

static std::queue<Event> events;

bool GetNextEvent(Event *event){
     // Event e = {};
     if(events.size() == 0) return false;

     Event e = events.front();
     event->key = e.key;
     event->action = e.action;
     events.pop();
     return true;

}
bool IsKeyPressed(Window *window, int key){
     int state = glfwGetKey(window->GLFWInstance, key);
     if (state == GLFW_PRESS)
     {
         return true;
     }
     return false;
}

void PollKeyboardEvents(GLFWwindow* window, int key, int scancode, int action, int mods)
{
     Event e = {key , action};
     events.push(e);
}

//TODO: Set here the different callbacks to glfw and change the name of the function.
void SetKeyboardCallback(Window *window){
     glfwSetKeyCallback(window->GLFWInstance, PollKeyboardEvents);
}

void PrintEvents(){
     for(int i = 0; i < events.size(); i++){
          Event e = events.front();
          printf("%d\n", e.key);
          printf("%d\n", e.action);
          printf("\n\n");
          events.pop();
     }
}
