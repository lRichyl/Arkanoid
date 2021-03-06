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
     assert(renderer);
     assert(w);
     powerUpProbability = powerUpInitialProbability;
     srand(time(NULL));
     InitLevels();
     // InitPowerUps();
     // test = CreatePowerUp(PowerUpType::POWER_LASER, V2 {100,600});

     assert(currentLevel);

     paddle.Init(V2{384, 16}, arkanoidTexture, renderer);
     Ball mainBall;
     mainBall.Init(V2{0, 0}, arkanoidTexture);
     mainBall.ResetPosition(&paddle);
     balls.push_back(mainBall);
     showFPS = true;
     timer.timeToWait = 2;
     FPSTimer.timeToWait = 0.5;
     laserShootTimer.timeToWait = 0.3;
     laserActiveTimer.timeToWait = 5;
     // catchTimer.timeToWait = 3;
}

void Game::InitLevels(){
     Level level1 = Level("Level 1", "assets/textures/Level 1 Background.png", {1.0f,0.1f,0.1f});
     Level level2 = Level("Level 2", "assets/textures/Level 1 Background.png", {0.1f,1.0f,0.1f});
     Level level3 = Level("Level 3", "assets/textures/Level 1 Background.png", {0.1f,0.1f,1.0f});
     Level level4 = Level("Level 4", "assets/textures/Level 1 Background.png", {1.0f,0.2f,1.0f});
     Level level5 = Level("Level 5", "assets/textures/Level 1 Background.png", {0.8f,0.8f,0.1f});

     level1.layout = {1,1,1,1,1,1,1,1,1,1,1,1,
                      2,2,2,2,2,2,2,2,2,2,2,2,
                      3,3,3,3,3,3,3,3,3,3,3,3,
                      4,4,4,4,4,4,4,4,4,4,4,4,
                      5,5,5,5,5,5,5,5,5,5,5,5,
                      6,6,6,6,6,6,6,6,6,6,6,6,
                      7,7,7,7,7,7,7,7,7,7,7,7};
     level2.layout = {1,1,1,1,1,0,0,0,0,0,0,0,
                      2,2,2,2,2,2,0,0,0,0,0,0,
                      3,3,3,3,3,3,3,0,0,0,0,0,
                      4,4,4,4,4,4,4,4,0,0,0,0,
                      5,5,5,5,5,5,5,5,5,0,0,0,
                      6,6,6,6,6,6,6,6,6,6,0,0,
                      1,7,1,7,1,7,1,7,1,7,1,7};
     level3.layout = {1,0,1,0,1,0,1,0,1,0,1,0,
                      2,2,2,2,2,2,2,2,2,2,2,2,
                      3,1,3,1,3,1,3,1,3,1,3,1,
                      4,4,4,4,4,4,4,4,4,4,4,4,
                      5,1,5,1,5,1,5,1,5,1,5,1,
                      6,6,6,6,6,6,6,6,6,6,6,6,
                      1,7,1,7,1,7,1,7,1,7,1,7};
     level4.layout = {1,4,4,4,4,4,4,4,4,4,4,1,
                      5,1,5,5,5,5,5,5,5,5,1,5,
                      2,2,1,2,2,2,2,2,2,1,2,2,
                      3,3,3,1,3,3,3,3,1,3,3,3,
                      7,7,7,7,1,1,1,1,7,7,7,7,
                      6,1,1,1,1,1,1,1,1,1,1,6,
                      6,1,1,1,1,1,1,1,1,1,1,6};
     level5.layout = {1,1,1,1,1,1,1,1,1,1,1,1,
                      1,1,4,4,4,4,4,4,4,4,1,1,
                      1,7,1,7,7,7,7,7,7,1,7,1,
                      1,6,6,1,6,6,6,6,1,6,6,1,
                      1,3,3,3,1,3,3,1,3,3,3,1,
                      1,2,2,2,2,1,1,2,2,2,2,1,
                      1,5,1,5,1,5,5,1,5,1,5,1};

     levelList.push_back(level1);
     levelList.push_back(level2);
     levelList.push_back(level3);
     levelList.push_back(level4);
     levelList.push_back(level5);

     currentLevel = &levelList[0];
}

// void Game::InitPowerUps(){
//      laserPower.Init(Rect {32, 16, 32, 16}, arkanoidTexture, PowerUpType::POWER_LASER);
//      enlargePower.Init(Rect {64, 16, 32, 16}, arkanoidTexture, PowerUpType::POWER_ENLARGE);
//      catchPower.Init(Rect {96, 16, 32, 16}, arkanoidTexture, PowerUpType::POWER_CATCH);
//      slowPower.Init(Rect {128, 16, 32, 16}, arkanoidTexture, PowerUpType::POWER_SLOW);
//      disruptionPower.Init(Rect {160, 16, 32, 16}, arkanoidTexture, PowerUpType::POWER_DISRUPTION);
//      extraPlayerPower.Init(Rect {192, 16, 32, 16}, arkanoidTexture, PowerUpType::POWER_EXTRA_PLAYER);
// }

PowerUp Game::CreatePowerUp(PowerUpType type, V2 position){
     PowerUp power;
     switch(type){
          case POWER_LASER:        power.Init(position, Rect {32, 16, 32, 16}, arkanoidTexture, PowerUpType::POWER_LASER);break;
          case POWER_ENLARGE:      power.Init(position, Rect {64, 16, 32, 16}, arkanoidTexture, PowerUpType::POWER_ENLARGE); break;
          case POWER_CATCH:        power.Init(position, Rect {96, 16, 32, 16}, arkanoidTexture, PowerUpType::POWER_CATCH); break;
          case POWER_SLOW:         power.Init(position, Rect {128, 16, 32, 16}, arkanoidTexture, PowerUpType::POWER_SLOW); break;
          case POWER_DISRUPTION:   power.Init(position, Rect {160, 16, 32, 16}, arkanoidTexture, PowerUpType::POWER_DISRUPTION); break;
          case POWER_EXTRA_PLAYER: power.Init(position, Rect {192, 16, 32, 16}, arkanoidTexture, PowerUpType::POWER_EXTRA_PLAYER); break;
          default: power.type = (PowerUpType)0;
     }

     return power;
}

void Game::DoLaserPowerUp(){
     if(powerUpFlags & PowerUpType::POWER_LASER){
          laserActiveTimer.Tick();
          if(canShootLaser && IsKeyPressed(renderer->window, GLFW_KEY_SPACE)){
               canShootLaser = false;

               Laser laser;
               V2 position = {(paddle.boundingBox.w / 4.0f) + paddle.boundingBox.x, paddle.boundingBox.y + paddle.boundingBox.h + 1.0f};
               laser.Init(position, arkanoidTexture);
               Laser laser1;
               V2 position1 = {(paddle.boundingBox.w / 4.0f * 3.0f) + paddle.boundingBox.x, paddle.boundingBox.y + paddle.boundingBox.h + 1.0f};
               laser1.Init(position1, arkanoidTexture);

               lasers.push_back(laser);
               lasers.push_back(laser1);
          }else if(!canShootLaser){
               laserShootTimer.Tick();
               if(laserShootTimer.isTimeReached)
                    canShootLaser = true;
          }

          if(laserActiveTimer.isTimeReached){
               // printf("A%x\n", powerUpFlags);

               powerUpFlags &=  ~(1 << (PowerUpType::POWER_LASER >> 1));
               // printf("B%x\n", powerUpFlags);
          }

     }


}

void Level::DrawBackground(Renderer *renderer){
     render_quad(renderer,NULL, &background, NULL, false, 255, backgroundTint);
}

void Game::UpdateGame(float dt){
     switch(state){
          case GAME_LEVEL_LOADING:{
               CalculateNumberOfBlocksToWin();
               GenerateBlocksBoundingBoxes();
               ResetBlocksState();
               powerUpsOnScreen.clear();
               lasers.clear();
               paddle.ResetPosition(window);
               for(int i = 0; i < balls.size(); i++){
                    balls[i].ResetPosition(&paddle);
                    balls[i].ResetSpeed();
               }
               // ball.ResetPosition(&paddle);
               state = GameState::GAME_PLAYING;
               break;
          }
          case GAME_PLAYING:{
               // powerUpFlags |= PowerUpType::POWER_LASER;
               DoEvents();

               paddle.Update(dt, renderer);

               for(int i = 0; i < balls.size(); i++){
                    balls[i].Update(dt, renderer, &paddle);
               }
               for(int i = 0; i < powerUpsOnScreen.size(); i++){
                    PowerUp *p = &powerUpsOnScreen[i];
                    powerUpsOnScreen[i].Update(dt);

                    if(p->boundingBox.y < 0) powerUpsOnScreen.erase(powerUpsOnScreen.begin() + i);
               }
               for(int i = 0; i < lasers.size(); i++){
                    lasers[i].Update(dt);
                    if(lasers[i].boundingBox.y - lasers[i].boundingBox.h > window->internalHeight) lasers.erase(lasers.begin() + i);
               }

               DoLaserPowerUp();
               MaybeLaunchBall();
               ResetPowerUpsAndBallIfBallFalls();

               //Collisions
               BallCollisionWithBlocks(dt);
               BallCollisionWithPaddle(dt);
               PowerUpCollisionWithPaddle();
               LaserCollisionWithBlocks();
               if(numberOfBlocksToWin == 0){
                    if(nextLevel < levelList.size()){
                         // nextLevel++;
                         // currentLevel = &levelList[nextLevel];
                         state = GameState::GAME_LEVEL_TRANSITION;
                    }else{
                         //All level were completed.
                    }
               }


               break;
          }
          case GAME_LEVEL_TRANSITION:{
               timer.Tick();
               if(timer.isTimeReached){
                    nextLevel++;
                    currentLevel = &levelList[nextLevel];
                    state = GameState::GAME_LEVEL_LOADING;
               }
               break;
          }
     }
}



void Game::DrawGame(float dt, float fps){
     currentLevel->DrawBackground(renderer);
     DrawCurrentLevel();
     std::string blocksToWinString = std::to_string(numberOfBlocksToWin);
     render_text(renderer, &debugFont, &blocksToWinString, V2{400, 600}, V3{1.0f,1.0f,1.0f}, true);

     FPSTimer.Tick();
     static std::string fpsString;
     int fpsInt = (int)fps;
     fpsString = std::to_string(fpsInt);
     if(showFPS){
          render_text(renderer, &FPSFont, &fpsString, V2{0, (float)window->internalHeight}, V3{0.0f,1.0f,0.0f});
     }

     if(state == GameState::GAME_LEVEL_TRANSITION && !timer.isTimeReached){
          static std::string completedString = "COMPLETED";
          render_text(renderer, &debugFont, &currentLevel->name, V2{400, 300}, V3{1.0f,1.0f,1.0f}, true);
          render_text(renderer, &debugFont, &completedString, V2{400, 270}, V3{1.0f,1.0f,1.0f}, true);
     }


     for(int i = 0; i < balls.size(); i++){
          balls[i].Draw(renderer);

     }
     paddle.Draw(renderer);
     for(int i = 0; i < powerUpsOnScreen.size(); i++){
          powerUpsOnScreen[i].Draw(renderer);
     }

     for(int i = 0; i < lasers.size(); i++){
          lasers[i].Draw(renderer);
     }


     renderer_draw(renderer);
     swap_buffers(window);
}

void Game::GameLoop(float dt, float fps){
     UpdateGame(dt);
     DrawGame(dt, fps);
     poll_events();
}

void Game::DoEvents(){
     Event e;
     while(GetNextEvent(&e)){
          #if DEV
               if(e.key == GLFW_KEY_C && e.action == GLFW_PRESS){
                    // printf("SUCCESS\n");
                    ClearLevel();
               }
          #endif

     }
}

void Game::ResetPowerUpsAndBallIfBallFalls(){
     if(balls.size() == 1){
          if(balls[0].boundingBox.y < 0){
               balls[0].state = BallState::ON_PADDLE;
               balls[0].ResetSpeed();
               powerUpFlags = 0;
               powerUpProbability = powerUpInitialProbability;

               paddle.ResetSize();
          }

     }else if(balls.size() > 1){
          for(int i = 0; i < balls.size(); i++){
               if(balls[i].boundingBox.y < 0){
                    balls.erase(balls.begin() + i);
               }

          }
     }
}


void Game::MaybeLaunchBall(){
     for(int i = 0; i < balls.size(); i++){
          if(balls[i].state == BallState::ON_PADDLE || balls[i].state == BallState::ON_CATCH){
               if(powerUpFlags & PowerUpType::POWER_CATCH){
                    balls[i].catchTimer.Tick();
               }

               // printf("time: %f\n", catchTimer.timeCount);
               if(IsKeyPressed(window, GLFW_KEY_SPACE) || balls[i].catchTimer.isTimeReached){
                    float bounceCoefficient = CalculateBallBounceCoefficient(&balls[i]);

                    balls[i].velocity.x = balls[i].speed * bounceCoefficient;
                    balls[i].velocity.y = balls[i].speed;
                    balls[i].state = BallState::MOVING;
               }

          }

     }
}

void Game::GenerateBlocksBoundingBoxes(){
     assert((Level::levelHeight * Level::levelWidth) <= maxNumberOfBlocksBoundingBoxes);
     for(int y = 0; y < Level::levelHeight; y++){
          for(int x = 0; x < Level::levelWidth; x++){
               int index = (Level::levelWidth * y) + x;
               // printf("%d\n", currentLevel[index]);
               if(currentLevel->layout[index] != Blocks::BLOCKS_EMPTY){
                    Rect boundingBox = {levelOffset.x + x * blockSize.x, window->internalHeight - levelOffset.y - y * blockSize.y, blockSize.x, blockSize.y};
                    blocksBoundingBoxes[index] = boundingBox;
               }else{
                    blocksBoundingBoxes[index] = {0,0,0,0};
               }
          }
     }
}

void Game::ResetBlocksState(){
     for(int y = 0; y < Level::levelHeight; y++){
          for(int x = 0; x < Level::levelWidth; x++){
               int index = (Level::levelWidth * y) + x;
               // printf("%d\n", currentLevel[index]);
               if(currentLevel->layout[index] != Blocks::BLOCKS_EMPTY){
                    blockStateMap[index] = 1;
               }else{
                    blockStateMap[index] = 0;
               }
          }
     }
}

void Game::ClearLevel(){
     if(nextLevel < levelList.size() - 1){
          numberOfBlocksToWin = 0;
     }else{
          printf("There's no more levels\n");
     }

}

void Game::DrawCurrentLevel(){
     for(int y = 0; y < Level::levelHeight; y++){
          for(int x = 0; x < Level::levelWidth; x++){
               int index = (Level::levelWidth * y) + x;
               int clipRegion = currentLevel->layout[index];
               if(blockStateMap[index] > 0){
                    render_quad(renderer, &blocksBoundingBoxes[index], &arkanoidTexture, &blockClipRegions[clipRegion]);
               }
          }
     }
}

void Game::CalculateNumberOfBlocksToWin(){
     for(int y = 0; y < Level::levelHeight; y++){
          for(int x = 0; x < Level::levelWidth; x++){
               int index = (Level::levelWidth * y) + x;
               if(currentLevel->layout[index] != Blocks::BLOCKS_BLACK && currentLevel->layout[index] != Blocks::BLOCKS_EMPTY){
                    numberOfBlocksToWin++;
               }
          }
     }
}

void Game::BallCollisionWithBlocks(float dt){
     V2 penetration;
     for(int y = 0; y < Level::levelHeight; y++){
          for(int x = 0; x < Level::levelWidth; x++){
               int index = (Level::levelWidth * y) + x;
               if(blockStateMap[index]){
                    for(int i = 0; i < balls.size(); i++){
                         if(DoRectsCollide(balls[i].boundingBox, blocksBoundingBoxes[index], &penetration)){
                              //Black blocks are indestructible.
                              if(currentLevel->layout[index] != Blocks::BLOCKS_BLACK){
                                   blockStateMap[index] = 0;
                                   numberOfBlocksToWin--;

                                   int powerChance = rand() % 100;
                                   // printf("Chance %d\n", powerChance);
                                   if(powerChance < powerUpProbability){
                                        int power = (rand() % PowerUpType::POWER_COUNT);
                                        int finalPower = 1 << power;
                                        PowerUp selectedPowerUp = CreatePowerUp((PowerUpType)finalPower, {balls[i].boundingBox.x,balls[i].boundingBox.y});
                                        // printf("Power granted: %d\n\n", finalPower);
                                        // printf("Type: %x\n",selectedPowerUp.type);
                                        powerUpsOnScreen.push_back(selectedPowerUp);
                                   }
                                   powerUpProbability += 50;

                                   balls[i].SpeedUp();
                                   // printf("%f\n", ball.speed);
                              }
                              balls[i].Bounce(penetration);
                              return; //Thiis is done so that you can't destroy many blocks at the same time.
                         }

                    }
               }
          }
     }
}

float Game::CalculateBallBounceCoefficient(Ball *ball){
     float ballCenter = ball->boundingBox.x + ball->boundingBox.w / 2;
     float ballPositionRelativeToPaddle = ballCenter - paddle.boundingBox.x;
     float bounceCoefficient = (ballPositionRelativeToPaddle / (paddle.boundingBox.w / 2)) - 1;
     return bounceCoefficient;
}

void Game::BallCollisionWithPaddle(float dt){
     V2 penetration;
     for(int i = 0; i < balls.size(); i++){

          if(balls[i].state == BallState::MOVING){
               if(DoRectsCollide(balls[i].boundingBox, paddle.boundingBox, &penetration)){
                    if(powerUpFlags & PowerUpType::POWER_CATCH){
                         if(penetration.y < 0){
                              balls[i].boundingBox.y -= penetration.y;
                              balls[i].onCatchRelativePosition = balls[i].boundingBox.x - paddle.boundingBox.x;
                              balls[i].state = BallState::ON_CATCH;
                              balls[i].speed = balls[i].defaultSpeed;
                              balls[i].catchTimer.timeCount = 0;
                         }else{
                              balls[i].Bounce(penetration);
                         }
                    }else{
                         // ball.Bounce(penetration);
                         if(penetration.y < 0){
                              float bounceCoefficient = CalculateBallBounceCoefficient(&balls[i]);
                              float bounceSpeed;
                              // if(bounceCoefficient > 0 || bounceCoefficient < 0) bounceSpeed = ball.speed;
                              // else bounceSpeed = 0;

                              balls[i].velocity.x = balls[i].speed * bounceCoefficient;
                              balls[i].velocity.y = balls[i].speed;
                         }else{
                              balls[i].Bounce(penetration);
                         }

                    }


               }

          }
     }
}

void Game::PowerUpCollisionWithPaddle(){
     for(int i = 0; i < powerUpsOnScreen.size(); i++){
          PowerUp *p = &powerUpsOnScreen[i];
          V2 penetration;
          if(DoRectsCollide(p->boundingBox, paddle.boundingBox, &penetration)){
               powerUpFlags |= p->type;
               if(p->type == PowerUpType::POWER_ENLARGE){
                    paddle.Enlarge();
               }

               if(p->type == PowerUpType::POWER_SLOW){
                    for(int i = 0; i < balls.size(); i++){
                         balls[i].speed -= balls[i].speed * 0.5f;
                         balls[i].velocity.x *= 0.5f;
                         balls[i].velocity.y *= 0.5f;

                    }
               }

               if(p->type == PowerUpType::POWER_DISRUPTION){
                    Ball newBall;
                    Ball newBall1;
                    V2 position = {balls[0].boundingBox.x, balls[0].boundingBox.y};
                    newBall.Init(position, arkanoidTexture);
                    newBall1.Init(position, arkanoidTexture);

                    float speedBall = (rand() % ((int)balls[0].speed - 199)) + 201;
                    float speedBall1 = -(rand() % ((int)balls[0].speed - 199)) + 201;
                    newBall.speed = speedBall;
                    newBall1.speed = speedBall1;

                    newBall.state = BallState::MOVING;
                    newBall.velocity.x = newBall.speed;
                    newBall.velocity.y = balls[0].velocity.y;

                    newBall1.state = BallState::MOVING;
                    newBall1.SetVelocityToSpeed();
                    newBall1.SetVelocityToSpeed();
                    // printf("%f, %f\n", speedBall, speedBall1);

                    balls.push_back(newBall);
                    balls.push_back(newBall1);
               }

               powerUpsOnScreen.erase(powerUpsOnScreen.begin() + i);
          }
     }
}

void Game::LaserCollisionWithBlocks(){
     V2 penetration;
     for(int y = 0; y < Level::levelHeight; y++){
          for(int x = 0; x < Level::levelWidth; x++){
               int index = (Level::levelWidth * y) + x;
               if(blockStateMap[index]){
                    for(int i = 0; i < lasers.size(); i++){
                         Laser *laser = &lasers[i];
                         if(DoRectsCollide(laser->boundingBox, blocksBoundingBoxes[index], &penetration)){
                              lasers.erase(lasers.begin() + i);
                              if(currentLevel->layout[index] != Blocks::BLOCKS_BLACK){
                                   blockStateMap[index] = 0;
                                   numberOfBlocksToWin--;
                              }
                         }

                    }
               }
          }
     }
}
