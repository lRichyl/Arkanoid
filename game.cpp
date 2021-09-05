#include "renderer.h"
#include "game.h"
#include "GLFW/glfw3.h"
#include "math.h"

Game::Game(){

}
//Move this to another file to handle events if necessary
bool isKeyPressed(Window *window, int key){
     int state = glfwGetKey(window->w, key);
     if (state == GLFW_PRESS)
     {
         return true;
     }
     return false;
}

//This should be moved to a paddle object
Rect position = {384,16,64,16};
Rect clip = {0 , 0, 32 , 8};
V2 direction = {};

void Game::UpdateGame(float dt, Renderer *renderer){
     if(isKeyPressed(renderer->window, GLFW_KEY_D)){
          direction.x = 1;
     }
     if(isKeyPressed(renderer->window, GLFW_KEY_A)){
          direction.x = -1;
     }
     if(!isKeyPressed(renderer->window, GLFW_KEY_D) && !isKeyPressed(renderer->window, GLFW_KEY_A)
          || isKeyPressed(renderer->window, GLFW_KEY_D) && isKeyPressed(renderer->window, GLFW_KEY_A)){
          direction.x = 0;
     }

     // if(isKeyPressed(renderer->window, GLFW_KEY_W)){
     //      direction.y = 1;
     // }
     // if(isKeyPressed(renderer->window, GLFW_KEY_S)){
     //      direction.y = -1;
     // }
     // if(!isKeyPressed(renderer->window, GLFW_KEY_W) && !isKeyPressed(renderer->window, GLFW_KEY_S)
     //      || isKeyPressed(renderer->window, GLFW_KEY_W) && isKeyPressed(renderer->window, GLFW_KEY_S)){
     //      direction.y = 0;
     // }

     position.x += 100 * dt * direction.x;
     // position.y += 100 * dt * direction.y;

}

void Game::DrawGame(Renderer *renderer, Window *window){
     //Right now we only draw the main batch, which can have 1000 quads with as many textures units as the
     //system can handle.
     //TODO: Add more batches to draw on when we run out of texture units or surpass 1000 quads.
     render_quad(renderer, &position, &arkanoidBlocks, &clip);
     renderer_draw(renderer);

     swap_buffers(window);
}

void Game::GameLoop(float dt, Renderer *renderer, Window *window){
     UpdateGame(dt, renderer);
     DrawGame(renderer, window);
     poll_events();
}
