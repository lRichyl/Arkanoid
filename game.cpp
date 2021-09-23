#include "renderer.h"
#include "game.h"
#include "GLFW/glfw3.h"
#include "entities.h"
#include "math.h"
#include "collision.h"
#include "input.h"
#include "text.h"
#include "stb_truetype.h"


// stbtt_bakedchar cdata[96]; // ASCII 32..126 is 95 glyphs
// GLuint ftex;
//
//
// void my_stbtt_initfont(Texture *tex)
// {
//      unsigned char ttf_buffer[1<<15];
//      unsigned char temp_bitmap[512*512];
//
//      fread(ttf_buffer, 1, 1<<15, fopen("assets/fonts/NugoSansLight.ttf", "rb"));
//      stbtt_BakeFontBitmap(ttf_buffer,0, 32.0, temp_bitmap,512,512, 32,96, cdata); // no guarantee this fits!
//      // can free ttf_buffer at this point
//      glGenTextures(1, &ftex);
//      glBindTexture(GL_TEXTURE_2D, ftex);
//      glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 512,512, 0, GL_RED, GL_UNSIGNED_BYTE, temp_bitmap);
//      // can free temp_bitmap at this point
//      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//      tex->id = ftex;
// }


Game::Game(Renderer *r, Window *w){
     renderer = r;
     window = w;
     levelList[0] = &level1[0];
     currentLevel = levelList[0];
     assert(currentLevel);
     CalculateNumberOfBlocksToWin(); //This should be calculated once at the start of every level
     GenerateBlocksBoundingBoxes();
     paddle.Init(V2{384, 16}, arkanoidTexture);
     ball.Init(V2{0, 0}, arkanoidTexture);
     ball.ResetPosition(&paddle);
}

void Game::UpdateGame(float dt){
     //When we load a new level the numberOfBlocksToWin and the blocksBoundingBoxes must be
     //recalculated.

     paddle.Update(dt, renderer);
     ball.Update(dt, renderer, &paddle);
     MaybeLaunchBall();

     //Collisions
     BallCollisionWithBlocks(dt);
     BallCollisionWithPaddle(dt);
}

Rect p = {0,600,512,512};
void Game::DrawGame(){
     paddle.Draw(renderer);
     ball.Draw(renderer);
     DrawCurrentLevel();

     // render_quad_to_ui(renderer, &p, &test.texture, NULL, false, 50);
     // render_text(renderer, &test, "Esta es una prueba de texto", V2 {0 , 600}, 32);

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
                    if(DoRectsCollide(ball.boundingBox, blocksBoundingBoxes[index], &penetration)){
                         blockStateMap[index] = 0;
                         numberOfBlocksToWin--;;
                         ball.Bounce(penetration);
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
