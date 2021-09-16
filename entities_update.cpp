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
     else if(boundingBox.x + boundingBox.w + pendingMovement > renderer->window->internalWidth){
          boundingBox.x = renderer->window->internalWidth - boundingBox.w;
     }
     else{
          boundingBox.x += pendingMovement;

     }
}

void Ball::Update(float dt, Renderer *renderer, Paddle *paddle){
     switch(state){
          case ON_PADDLE:{
               velocity.x = 0;
               velocity.y = speed;
               ResetPosition(paddle);
               break;
          }
          case MOVING:{
               boundingBox.x += velocity.x * dt;
               boundingBox.y += velocity.y * dt;

               if(boundingBox.y < 0 || boundingBox.y - boundingBox.h > renderer->window->internalHeight){
                    ResetPosition(paddle);
                    state = BallState::ON_PADDLE;
               }

               if(boundingBox.x < 0 || boundingBox.x + boundingBox.w > renderer->window->internalWidth){
                    velocity.x *= -1;
               }

               if(boundingBox.y - boundingBox.h > renderer->window->internalHeight){
                    velocity.y *= -1;
               }
               break;
          }
     }

}

void Ball::ResetPosition(Paddle *paddle){
     float x = paddle->boundingBox.x + paddle->boundingBox.w/2 - boundingBox.w/2;
     float y = paddle->boundingBox.y + boundingBox.h;
     boundingBox.x = x;
     boundingBox.y = y;
}

void Ball::Bounce(V2 penetration){
     if(penetration.y > 0 || penetration.y < 0) {
          boundingBox.y -= penetration.y;
          velocity.y *= -1;
     }

     if(penetration.x > 0 || penetration.x < 0){
          boundingBox.x -= penetration.x;
          velocity.x *= -1;
     }
}
