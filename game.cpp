#include "renderer.h"
#include "game.h"
#include "GLFW/glfw3.h"
#include "math.h"

Game::Game(){
     // paddle.boundingBox = {384,16,64,16};
     // paddle.clippingBox = {0 , 0, 32 , 8};
     paddle.Init(V2{384, 16});
}

void Game::UpdateGame(float dt, Renderer *renderer){
     // Entity *e = static_cast<Entity *>(&paddle);
     paddle.Update(dt, renderer);

}

void Game::DrawGame(Renderer *renderer, Window *window){
     //Right now we only draw the main batch, which can have 1000 quads with as many textures units as the
     //system can handle.
     //TODO: Add more batches to draw on when we run out of texture units or surpass 1000 quads.
     paddle.Draw(renderer);
     renderer_draw(renderer);

     swap_buffers(window);
}

void Game::GameLoop(float dt, Renderer *renderer, Window *window){
     UpdateGame(dt, renderer);
     DrawGame(renderer, window);
     poll_events();
}
