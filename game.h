#ifndef GAME_H
#define GAME_H
#include "renderer.h"
#include "entities.h"
#include "math.h"
#include <vector>
#include <array>

struct Game{
     void UpdateGame(float dt);
     void DrawGame();
     void GameLoop(float dt);
     Game(Renderer *r, Window *w);

     void DrawCurrentLevel();
     void DrawBall();
     void CalculateNumberOfBlocksToWin();
     void DoBallCollisionWithBlocks(float dt);

     Renderer *renderer;
     Window *window;
     Texture arkanoidTexture = make_texture("assets/textures/Arkanoid blocks.png");
     Rect blockClipRegions[Blocks::BLOCKS_COUNT] =
                               {Rect {32, 0, 32, 16},
                                Rect {64, 0, 32, 16},
                                Rect {96, 0, 32, 16},
                                Rect {128,0, 32, 16},
                                Rect {160,0, 32, 16},
                                Rect {192,0, 32, 16},
                                Rect {224,0, 32, 16}};
     Paddle paddle;
     Ball ball;
     static const int levelWidth = 12;
     static const int levelHeight = 7;
     static const int numberOfLevels = 5;
     static constexpr V2 blockSize = {63, 32};
     int *levelList[numberOfLevels];
     int *currentLevel;
     int numberOfBlocksToWin = 0;
     V2 levelOffset = {22, 30};

     int level1[levelWidth * levelHeight] ={0,0,0,0,0,0,0,0,0,0,0,0,
                                     1,1,1,1,1,1,1,1,1,1,1,1,
                                     2,2,2,2,2,2,2,2,2,2,2,2,
                                     3,3,3,3,3,3,3,3,3,3,3,3,
                                     4,4,4,4,4,4,4,4,4,4,4,4,
                                     5,5,5,5,5,5,5,5,5,5,5,5,
                                     6,6,6,6,6,6,6,6,6,6,6,6};



     int blockStateMap[levelWidth * levelHeight] ={1,1,1,1,1,1,1,1,1,1,1,1,
                                  1,1,1,1,1,1,1,1,1,1,1,1,
                                  1,1,1,1,1,1,1,1,1,1,1,1,
                                  1,1,1,1,1,1,1,1,1,1,1,1,
                                  1,1,1,1,1,1,1,1,1,1,1,1,
                                  1,1,1,1,1,1,1,1,1,1,1,1,
                                  1,1,1,1,1,1,1,1,1,1,1,1};


};



#endif
