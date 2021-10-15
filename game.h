#ifndef GAME_H
#define GAME_H

#define DEV 1
#include "time.h"
#include "renderer.h"
#include "entities.h"
#include "text.h"
#include "math.h"
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>


struct Level{
     Level(std::string name, char *path, V3 tint){
          this->name = name;
          if(path)
               background = make_texture(path);
          backgroundTint = tint;
     }
     void DrawBackground(Renderer* renderer);

     static const int levelWidth = 12;
     static const int levelHeight = 7;
     std::string name;
     std::vector<int> layout;
     Texture background;
     V3 backgroundTint = {1.0f,1.0f,1.0f};
};
enum GameState{
     GAME_LEVEL_LOADING,
     GAME_PLAYING,
     GAME_LEVEL_TRANSITION
};

struct Game{
     void UpdateGame(float dt);
     void DrawGame(float dt, float fps);
     void GameLoop(float dt, float fps);
     Game(Renderer *r, Window *w);

     void DrawCurrentLevel();
     void DoEvents();
     void InitLevels();
     // void InitPowerUps();
     void MaybeLoadNextLevel();
     void DrawBall();
     void ResetPowerUpsAndBallIfBallFalls();
     void CalculateNumberOfBlocksToWin();
     void GenerateBlocksBoundingBoxes();
     void ResetBlocksState();
     void BallCollisionWithBlocks(float dt);
     void BallCollisionWithPaddle(float dt);
     void PowerUpCollisionWithPaddle();
     void LaserCollisionWithBlocks();
     void MaybeLaunchBall();
     void DoLaserPowerUp();

     float CalculateBallBounceCoefficient(Ball *ball);
     PowerUp CreatePowerUp(PowerUpType type, V2 position);

     //Only for development
     void ClearLevel();

     GameState state = GameState::GAME_LEVEL_LOADING;
     Renderer *renderer;
     Window *window;
     Texture arkanoidTexture = make_texture("assets/textures/Arkanoid blocks.png");
     Font debugFont = Font("assets/fonts/Simvoni.ttf", 20);
     Font FPSFont = Font("assets/fonts/Simvoni.ttf", 32);
     Timer timer;
     Timer FPSTimer;

     int powerUpInitialProbability = 10;
     int powerUpProbability;
     int powerUpRandomNumber = 0;
     int powerUpFlags = 0;
     std::vector<PowerUp> powerUpsOnScreen;

     bool canShootLaser = true;
     std::vector<Laser> lasers;
     Timer laserShootTimer;
     Timer laserActiveTimer;

     // Timer catchTimer;
     // PowerUpType selectedPowerUp;
// Texture tex;
     Rect blockClipRegions[Blocks::BLOCKS_COUNT] =
                               {Rect {0, 0, 0, 0},
                                Rect {32, 0, 32, 16},
                                Rect {64, 0, 32, 16},
                                Rect {96, 0, 32, 16},
                                Rect {128,0, 32, 16},
                                Rect {160,0, 32, 16},
                                Rect {192,0, 32, 16},
                                Rect {224,0, 32, 16}};
     Paddle paddle;
     std::vector<Ball> balls;


     bool showFPS = false;
     // static const int levelWidth = 12;
     // static const int levelHeight = 7;
     static const int maxNumberOfBlocksBoundingBoxes = 100;
     static const int numberOfLevels = 5;
     static constexpr V2 blockSize = {63, 32};

     // Level *levelList[numberOfLevels];
     std::vector<Level> levelList;
     Level *currentLevel;
     int nextLevel = 0;
     int numberOfBlocksToWin = 0;
     std::string currentLevelString;
     Rect blocksBoundingBoxes[maxNumberOfBlocksBoundingBoxes];
     V2 levelOffset = {22, 30};


     // int level1[levelWidth * levelHeight] ={1,1,1,1,1,1,1,1,1,1,1,1,
     //                                        2,2,2,2,2,2,2,2,2,2,2,2,
     //                                        3,3,3,3,3,3,3,3,3,3,3,3,
     //                                        4,4,4,4,4,4,4,4,4,4,4,4,
     //                                        5,5,5,5,5,5,5,5,5,5,5,5,
     //                                        6,6,6,6,6,6,6,6,6,6,6,6,
     //                                        7,7,7,7,7,7,7,7,7,7,7,7};

     // int level2[levelWidth * levelHeight] ={1,1,1,1,1,0,0,0,0,0,0,0,
     //                                        2,2,2,2,2,2,0,0,0,0,0,0,
     //                                        3,3,3,3,3,3,3,0,0,0,0,0,
     //                                        4,4,4,4,4,4,4,4,0,0,0,0,
     //                                        5,5,5,5,5,5,5,5,5,0,0,0,
     //                                        6,6,6,6,6,6,6,6,6,6,0,0,
     //                                        1,7,1,7,1,7,1,7,1,7,1,7};

     int blockStateMap[Level::levelWidth * Level::levelHeight] ={1,1,1,1,1,1,1,1,1,1,1,1,
                                                   1,1,1,1,1,1,1,1,1,1,1,1,
                                                   1,1,1,1,1,1,1,1,1,1,1,1,
                                                   1,1,1,1,1,1,1,1,1,1,1,1,
                                                   1,1,1,1,1,1,1,1,1,1,1,1,
                                                   1,1,1,1,1,1,1,1,1,1,1,1,
                                                   1,1,1,1,1,1,1,1,1,1,1,1};


};



#endif
