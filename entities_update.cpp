#include "entities.h"
#include "input.h"
#include "renderer.h"
#include "game.h"

void Paddle::Update(float dt, Renderer *renderer){
     float pendingMovement = 0;
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

     pendingMovement += speed * dt * direction.x;
     if(boundingBox.x + pendingMovement < 0){
          boundingBox.x = 0;
     }
     else if(boundingBox.x + boundingBox.w + pendingMovement > renderer->window->width){
          boundingBox.x = renderer->window->width - boundingBox.w;
     }
     else{
          boundingBox.x += pendingMovement;

     }
}

void Ball::Update(float dt, Renderer *renderer){
     // V2 direction = {};
     // V2 pendingMovement = {};
     // if(isKeyPressed(renderer->window, GLFW_KEY_D)){
     //      direction.x = 1;
     // }
     // if(isKeyPressed(renderer->window, GLFW_KEY_A)){
     //      direction.x = -1;
     // }
     // if(isKeyPressed(renderer->window, GLFW_KEY_W)){
     //      direction.y = 1;
     // }
     // if(isKeyPressed(renderer->window, GLFW_KEY_S)){
     //      direction.y = -1;
     // }
     // if(!isKeyPressed(renderer->window, GLFW_KEY_D) && !isKeyPressed(renderer->window, GLFW_KEY_A)
     //      || isKeyPressed(renderer->window, GLFW_KEY_D) && isKeyPressed(renderer->window, GLFW_KEY_A)){
     //      direction.x = 0;
     // }
     //
     // pendingMovement.x += speed * dt * direction.x;
     // pendingMovement.y += speed * dt * direction.y;
     // boundingBox.x += pendingMovement.x;
     // boundingBox.y += pendingMovement.y;
     boundingBox.x += velocity.x * dt;
     boundingBox.y += velocity.y * dt;
}
