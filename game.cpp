#include "renderer.h"
#include "game.h"
#include "GLFW/glfw3.h"
#include "entities.h"
#include "math.h"
#include "collision.h"
#include "input.h"

Game::Game(Renderer *r, Window *w){
     renderer = r;
     window = w;
     levelList[0] = &level1[0];
     currentLevel = levelList[0];
     assert(currentLevel);
     CalculateNumberOfBlocksToWin(); //This should be calculated once at the start of every level
     GenerateBlocksBoundingBoxes();
     // paddle.boundingBox = {384,16,64,16};
     // paddle.clippingBox = {0 , 0, 32 , 8};
     paddle.Init(V2{384, 16}, arkanoidTexture);
     ball.Init(V2{0, 0}, arkanoidTexture);
     ball.ResetPosition(&paddle);
}

void Game::UpdateGame(float dt){
     paddle.Update(dt, renderer);
     ball.Update(dt, renderer, &paddle);
     MaybeLaunchBall();

     //Collisions
     BallCollisionWithBlocks(dt);
     BallCollisionWithPaddle(dt);
}

Rect p = {100,100,32,16};
void Game::DrawGame(){
     //Right now we only draw the main batch, which can have 1000 quads with as many textures units as the
     //system can handle.
     //TODO: Add more batches to draw on when we run out of texture units or surpass 1000 quads.
     paddle.Draw(renderer);
     ball.Draw(renderer);
     DrawCurrentLevel();
     render_quad_to_ui(renderer, &p, &arkanoidTexture);

     renderer_draw(renderer);
     swap_buffers(window);
}

void Game::GameLoop(float dt){
     UpdateGame(dt);
     DrawGame();
     poll_events();
}


void Game::MaybeLaunchBall(){
     if(!ball.state == BallState::ON_PADDLE) return;
     if(isKeyPressed(window, GLFW_KEY_SPACE)){
          // ball.velocity.y = ball.speed;
          float velocity = paddle.speed / 2 * paddle.direction.x;
          ball.velocity.x += velocity;
          ball.state = BallState::MOVING;
     }
}

void Game::GenerateBlocksBoundingBoxes(){
     assert((levelHeight * levelWidth) <= maxNumberOfBlocksBoundingBoxes);
     for(int y = 0; y < levelHeight; y++){
          for(int x = 0; x < levelWidth; x++){
               int index = (levelWidth * y) + x;
               Rect boundingBox = {levelOffset.x + x * blockSize.x, window->internalHeight - levelOffset.y - y * blockSize.y, blockSize.x, blockSize.y};
               blocksBoundingBoxes[index] = boundingBox;
          }
     }
}

void Game::DrawCurrentLevel(){
     for(int y = 0; y < levelHeight; y++){
          for(int x = 0; x < levelWidth; x++){
               int index = (levelWidth * y) + x;
               int clipRegion = currentLevel[index];
               if(blockStateMap[index]){
                    //These should be save on their own array and initialize before every level.
                    // Rect boundingBox =
                    render_quad(renderer, &blocksBoundingBoxes[index], &arkanoidTexture, &blockClipRegions[clipRegion]);
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

void Game::BallCollisionWithBlocks(float dt){
     V2 penetration;
     for(int y = 0; y < levelHeight; y++){
          for(int x = 0; x < levelWidth; x++){
               int index = (levelWidth * y) + x;
               if(blockStateMap[index]){
                    //Look ABOVE line 59.
                    // Rect boundingBox = {levelOffset.x + x * blockSize.x, window->internalHeight - levelOffset.y - y * blockSize.y, blockSize.x, blockSize.y};
                    if(DoRectsCollide(ball.boundingBox, blocksBoundingBoxes[index], &penetration)){
                         blockStateMap[index] = 0;
                    }
                    ball.Bounce(penetration);
                    // if(penetration.y > 0 || penetration.y < 0) {
                    //      ball.boundingBox.y -= penetration.y;
                    //      ball.velocity.y *= -1;
                    // }
                    //
                    // if(penetration.x > 0 || penetration.x < 0){
                    //      ball.boundingBox.x -= penetration.x;
                    //      ball.velocity.x *= -1;
                    // }
               }
          }
     }
}

void Game::BallCollisionWithPaddle(float dt){
     V2 penetration;
     if(DoRectsCollide(ball.boundingBox, paddle.boundingBox, &penetration)){
          ball.Bounce(penetration);
          // if(penetration.y > 0 || penetration.y < 0) {
          //      ball.boundingBox.y -= penetration.y;
          //      ball.velocity.y *= -1;
          // }
          //
          // if(penetration.x > 0 || penetration.x < 0){
          //      ball.boundingBox.x -= penetration.x;
          //      ball.velocity.x *= -1;
          // }
     }
}
