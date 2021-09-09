#include "renderer.h"
#include "game.h"
#include "GLFW/glfw3.h"
#include "math.h"
#include "entities.h"
#include "collision.h"

Game::Game(Renderer *r, Window *w){
     renderer = r;
     window = w;
     levelList[0] = &level1[0];
     currentLevel = levelList[0];
     assert(currentLevel);
     CalculateNumberOfBlocksToWin(); //This should be calculated once at the start of every level
     // paddle.boundingBox = {384,16,64,16};
     // paddle.clippingBox = {0 , 0, 32 , 8};
     paddle.Init(V2{384, 16}, arkanoidTexture);
     ball.Init(V2{393, 26}, arkanoidTexture);
}

void Game::UpdateGame(float dt){
     paddle.Update(dt, renderer);
     ball.Update(dt, renderer);

     //Collisions
     DoBallCollisionWithBlocks(dt);
}

// Rect p = {100,100,32,16};
void Game::DrawGame(){
     //Right now we only draw the main batch, which can have 1000 quads with as many textures units as the
     //system can handle.
     //TODO: Add more batches to draw on when we run out of texture units or surpass 1000 quads.
     paddle.Draw(renderer);
     ball.Draw(renderer);
     // render_quad(renderer, &ball.boundingBox, &arkanoidTexture, &ball.clippingBox);
     DrawCurrentLevel();
     // render_quad(renderer, &p, &arkanoidTexture, &blockClipRegions[Blocks::BLOCKS_RED]);
     renderer_draw(renderer);

     swap_buffers(window);
}

void Game::GameLoop(float dt){
     UpdateGame(dt);
     DrawGame();
     poll_events();
}

void Game::DrawCurrentLevel(){
     for(int y = 0; y < levelHeight; y++){
          for(int x = 0; x < levelWidth; x++){
               int index = (levelWidth * y) + x;
               int clipRegion = currentLevel[index];
               if(blockStateMap[index]){
                    Rect boundingBox = {levelOffset.x + x * blockSize.x, window->height - levelOffset.y - y * blockSize.y, blockSize.x, blockSize.y};
                    render_quad(renderer, &boundingBox, &arkanoidTexture, &blockClipRegions[clipRegion]);
               }
          }
     }
}

//Remember to do this calculation every time you change levels
void Game::CalculateNumberOfBlocksToWin(){
     for(int y = 0; y < levelHeight; y++){
          for(int x = 0; x < levelWidth; x++){
               int index = (levelWidth * y) + x;
               if(currentLevel[index] != Blocks::BLOCKS_BLACK){
                    numberOfBlocksToWin++;
               }
          }
     }
}

void Game::DoBallCollisionWithBlocks(float dt){
     V2 penetration; //We don't use this currently
     for(int y = 0; y < levelHeight; y++){
          for(int x = 0; x < levelWidth; x++){
               int index = (levelWidth * y) + x;
               if(blockStateMap[index]){
                    Rect boundingBox = {levelOffset.x + x * blockSize.x, window->height - levelOffset.y - y * blockSize.y, blockSize.x, blockSize.y};
                    if(DoRectsCollide(ball.boundingBox, boundingBox, &penetration)){
                         blockStateMap[index] = 0;
                         printf("Collision %f , %f \n", penetration.x, penetration.y);
                    }
                    if(penetration.y > 0) {
                         ball.velocity.y = -(ball.speed);

                    }
               }
          }
     }
}
