#include "entities.h"
#include "input.h"
#include "renderer.h"
// #include "game.h"

void Paddle::Update(float dt, Renderer *renderer){
     float pendingMovement = 0;
     if(IsKeyPressed(renderer->window, GLFW_KEY_D)){
          direction.x = 1;
     }
     if(IsKeyPressed(renderer->window, GLFW_KEY_A)){
          direction.x = -1;
     }
     if(!IsKeyPressed(renderer->window, GLFW_KEY_D) && !IsKeyPressed(renderer->window, GLFW_KEY_A)
          || IsKeyPressed(renderer->window, GLFW_KEY_D) && IsKeyPressed(renderer->window, GLFW_KEY_A)){
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

void Paddle::ResetPosition(Window *window){
     boundingBox.x = window->internalWidth/2.0f - boundingBox.w/2.0f;
}

void Paddle::Enlarge(){
     boundingBox.w = enlargedWidth;
}
void Paddle::ResetSize(){
     boundingBox.w = normalWidth;
}

void Ball::Update(float dt, Renderer *renderer, Paddle *paddle){
     switch(state){
          case ON_PADDLE:{
               // velocity.x = 0;
               // velocity.y = 0;
               ResetPosition(paddle);
               break;
          }
          case MOVING:{
               if(velocity.y >  300) velocity.y =  300;
               if(velocity.y < -300) velocity.y = -300;
               if(velocity.x >  300) velocity.x =  300;
               if(velocity.x < -300) velocity.x = -300;
               boundingBox.x += velocity.x * dt;
               boundingBox.y += velocity.y * dt;



               if(boundingBox.x < 0){
                    float outwardDistance = 0 - boundingBox.x;
                    boundingBox.x += outwardDistance;
                    velocity.x *= -1;
               }
               if(boundingBox.x + boundingBox.w > renderer->window->internalWidth){
                    float outwardDistance = boundingBox.x + boundingBox.w - renderer->window->internalWidth;
                    boundingBox.x -= outwardDistance;
                    velocity.x *= -1;
               }

               if(boundingBox.y > renderer->window->internalHeight){
                    float outwardDistance = boundingBox.y - renderer->window->internalHeight;
                    boundingBox.y -= outwardDistance;
                    velocity.y *= -1;
               }
               break;
          }
     }

}

void Ball::ResetPosition(Paddle *paddle){
     state = BallState::ON_PADDLE;
     float x = paddle->boundingBox.x + paddle->boundingBox.w/2 - boundingBox.w/2;
     float y = paddle->boundingBox.y + boundingBox.h;
     boundingBox.x = x;
     boundingBox.y = y + 1;
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

void PowerUp::Update(float dt){
     boundingBox.y += gravity * dt;
}

void Laser::Update(float dt){
     boundingBox.y += speed * dt;
}
