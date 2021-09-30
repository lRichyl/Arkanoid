// #include "renderer.h"
#include "game.h"
#include "GLFW/glfw3.h"
#include "entities.h"
#include "math.h"
#include "collision.h"
#include "input.h"
#include "text.h"
#include "utilities.h"
#include "stb_truetype.h"

Game::Game(Renderer *r, Window *w){
     renderer = r;
     window = w;
     levelList[0] = &level1[0];
     levelList[1] = &level2[0];
     currentLevel = levelList[0];
     assert(currentLevel);

     paddle.Init(V2{384, 16}, arkanoidTexture);
     ball.Init(V2{0, 0}, arkanoidTexture);
     ball.ResetPosition(&paddle);
     showFPS = true;
     timer.timeToWait = 2;
}

void Game::UpdateGame(float dt){
     //When we load a new level the numberOfBlocksToWin and the blocksBoundingBoxes must be
     //recalculated.
     switch(state){
          case GAME_LEVEL_LOADING:{
               CalculateNumberOfBlocksToWin(); //This should be calculated once at the start of every level
               GenerateBlocksBoundingBoxes();
               ResetBlocksState();
               state = GameState::GAME_PLAYING;
               break;
          }
          case GAME_PLAYING:{
               timer.Tick();
               if(timer.isTimeReached){
                    printf("Time Reached \n");
               }
               if(isKeyPressed(renderer->window, GLFW_KEY_SPACE)){
                    timer.Stop();
               }
               if(isKeyPressed(renderer->window, GLFW_KEY_P)){
                    timer.Resume();
               }
               if(isKeyPressed(renderer->window, GLFW_KEY_Z)){
                    timer.Pause();
               }
               std::string blocksToWinString = std::to_string(numberOfBlocksToWin);
               // render_text(renderer, &debugFont, "HOLA MUNDO", V2{400, 300}, V3{1.0f,1.0f,1.0f}, true);
               render_text(renderer, &debugFont, blocksToWinString.c_str(), V2{400, 600}, V3{1.0f,1.0f,1.0f}, true);
               paddle.Update(dt, renderer);
               ball.Update(dt, renderer, &paddle);
               MaybeLaunchBall();

               //Collisions
               BallCollisionWithBlocks(dt);
               BallCollisionWithPaddle(dt);
               if(numberOfBlocksToWin == 0){
                    if(nextLevel < numberOfLevels){
                         currentLevel = levelList[nextLevel];
                         nextLevel++;
                         assert(nextLevel < numberOfLevels);
                         state = GameState::GAME_LEVEL_LOADING;
                    }else{
                         //All level were completed.
                    }
               }
          }
               break;
          }
     }



void Game::DrawGame(){
     paddle.Draw(renderer);
     ball.Draw(renderer);
     DrawCurrentLevel();
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
          float xVelocity = ball.speed / 2 * paddle.direction.x;
          ball.velocity.y = ball.speed;
          ball.velocity.x = xVelocity;
          ball.state = BallState::MOVING;
     }
}

//Generate this every time a new level is loaded.
void Game::GenerateBlocksBoundingBoxes(){
     assert((levelHeight * levelWidth) <= maxNumberOfBlocksBoundingBoxes);
     for(int y = 0; y < levelHeight; y++){
          for(int x = 0; x < levelWidth; x++){
               int index = (levelWidth * y) + x;
               // printf("%d\n", currentLevel[index]);
               if(currentLevel[index] != Blocks::BLOCKS_EMPTY){
                    Rect boundingBox = {levelOffset.x + x * blockSize.x, window->internalHeight - levelOffset.y - y * blockSize.y, blockSize.x, blockSize.y};
                    blocksBoundingBoxes[index] = boundingBox;
               }else{
                    blocksBoundingBoxes[index] = {0,0,0,0};
               }
          }
     }
}

void Game::ResetBlocksState(){
     for(int y = 0; y < levelHeight; y++){
          for(int x = 0; x < levelWidth; x++){
               int index = (levelWidth * y) + x;
               // printf("%d\n", currentLevel[index]);
               if(currentLevel[index] != Blocks::BLOCKS_EMPTY){
                    blockStateMap[index] = 1;
               }else{
                    blockStateMap[index] = 0;
               }
          }
     }
}

void Game::DrawCurrentLevel(){
     for(int y = 0; y < levelHeight; y++){
          for(int x = 0; x < levelWidth; x++){
               int index = (levelWidth * y) + x;
               int clipRegion = currentLevel[index];
               if(blockStateMap[index] > 0){
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
               if(currentLevel[index] != Blocks::BLOCKS_BLACK && currentLevel[index] != Blocks::BLOCKS_EMPTY){
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
                    if(DoRectsCollide(ball.boundingBox, blocksBoundingBoxes[index], &penetration)){
                         //Black blocks are indestructible.
                         if(currentLevel[index] != Blocks::BLOCKS_BLACK){
                              blockStateMap[index] = 0;
                              numberOfBlocksToWin--;
                         }
                         ball.Bounce(penetration);
                         // Bounce(&ball.boundingBox,&ball.velocity, penetration);
                         return; //Thiis is done so that you can't destroy many blocks at the same time.
                    }
               }
          }
     }
}

void Game::BallCollisionWithPaddle(float dt){
     V2 penetration;
     if(!ball.state == BallState::MOVING) return;
     if(DoRectsCollide(ball.boundingBox, paddle.boundingBox, &penetration)){
          ball.Bounce(penetration);
          float ballCenter = ball.boundingBox.x + ball.boundingBox.w / 2;
          float ballPositionRelativeToPaddle = ballCenter - paddle.boundingBox.x;
          float bounceCoefficient = (ballPositionRelativeToPaddle / (paddle.boundingBox.w / 2)) - 1;
          // printf("ball center: %f\n", ballCenter);
          // printf("ball position relative: %f\n", ballPositionRelativeToPaddle);
          // printf("bounce coefficient: %f\n", bounceCoefficient);
          float bounceSpeed;

          if(paddle.direction.x == 0){
               bounceSpeed = ball.speed;

               if (bounceCoefficient < 0) {
                    bounceSpeed *= -1;
               }
               else if(bounceCoefficient == 0){
                    bounceSpeed = 0;
               }
               // printf("bounce coefficient: %f\n", bounceCoefficient);
               ball.velocity.x = bounceSpeed;
          }else{
               if(bounceCoefficient > 0 || bounceCoefficient < 0) bounceSpeed = paddle.speed;
               else bounceSpeed = 0;
               float xVelocity = sqrt(pow(ball.velocity.y, 2) + pow(bounceSpeed, 2));
               xVelocity *= bounceCoefficient;
               xVelocity += paddle.speed * paddle.direction.x;
               ball.velocity.x = xVelocity;
          }


     }
}
