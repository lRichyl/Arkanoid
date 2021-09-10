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

void Ball::Update(float dt, Renderer *renderer, Paddle *paddle){
     boundingBox.x += velocity.x * dt;
     boundingBox.y += velocity.y * dt;

     if(boundingBox.y < 0) ResetPosition(paddle);
}

void Ball::ResetPosition(Paddle *paddle){
     float x = paddle->boundingBox.x + paddle->boundingBox.w/2 - boundingBox.w/2;
     float y = paddle->boundingBox.y + boundingBox.h;
     boundingBox.x = x;
     boundingBox.y = y;
}
