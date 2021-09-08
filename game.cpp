#include "renderer.h"
#include "game.h"
#include "GLFW/glfw3.h"
#include "math.h"
#include "entities.h"

Game::Game(Renderer *r, Window *w){
     renderer = r;
     window = w;
     // paddle.boundingBox = {384,16,64,16};
     // paddle.clippingBox = {0 , 0, 32 , 8};
     paddle.Init(V2{384, 16}, arkanoidTexture);
     ball.Init(V2{393, 26}, arkanoidTexture);
}

void Game::UpdateGame(float dt){
     paddle.Update(dt, renderer);

}

Rect p = {100,100,32,16};
void Game::DrawGame(){
     //Right now we only draw the main batch, which can have 1000 quads with as many textures units as the
     //system can handle.
     //TODO: Add more batches to draw on when we run out of texture units or surpass 1000 quads.
     paddle.Draw(renderer);
     ball.Draw(renderer);
     DrawLevel();
     // render_quad(renderer, &p, &arkanoidTexture, &blockClipRegions[Blocks::BLOCKS_RED]);
     renderer_draw(renderer);

     swap_buffers(window);
}

void Game::GameLoop(float dt){
     UpdateGame(dt);
     DrawGame();
     poll_events();
}
V2 offset = {22, 30};
void Game::DrawLevel(){
     for(int y = 0; y < levelHeight; y++){
          for(int x = 0; x < levelWidth; x++){
               int clipRegion = level1[(levelWidth * y) + x];
               Rect boundingBox = {offset.x + x * blockSize.x, window->height - offset.y - y * blockSize.y, blockSize.x, blockSize.y};
               render_quad(renderer, &boundingBox, &arkanoidTexture, &blockClipRegions[clipRegion]);
          }
     }
}
