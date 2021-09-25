#ifndef GAME_H
#define GAME_H
#include "renderer.h"
#include "entities.h"
#include "text.h"
#include "math.h"
#include <vector>
#include <array>

enum GameState{
     GAME_LEVEL_LOADING,
     GAME_PLAYING
};

struct Game{
     void UpdateGame(float dt);
     void DrawGame();
     void GameLoop(float dt);
     Game(Renderer *r, Window *w);

     void DrawCurrentLevel();
     void DrawBall();
     void CalculateNumberOfBlocksToWin();
     void GenerateBlocksBoundingBoxes();
     void ResetBlocksState();
     void BallCollisionWithBlocks(float dt);
     void BallCollisionWithPaddle(float dt);
     void MaybeLaunchBall();

     GameState state = GameState::GAME_LEVEL_LOADING;
     Renderer *renderer;
     Window *window;
     Texture arkanoidTexture = make_texture("assets/textures/Arkanoid blocks.png");
     Font debugFont = Font("assets/fonts/Simvoni.ttf", 20);
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
     Ball ball;
     bool showFPS = false;
     static const int levelWidth = 12;
     static const int levelHeight = 7;
     static const int maxNumberOfBlocksBoundingBoxes = 100;
     static const int numberOfLevels = 5;
     static constexpr V2 blockSize = {63, 32};

     int *levelList[numberOfLevels];
     int *currentLevel;
     int numberOfBlocksToWin = 0;
     Rect blocksBoundingBoxes[maxNumberOfBlocksBoundingBoxes];
     V2 levelOffset = {22, 30};

     int level1[levelWidth * levelHeight] ={1,1,1,1,1,1,1,1,1,1,1,1,
                                            0,0,0,0,0,0,0,2,2,2,2,2,
                                            0,0,0,0,0,0,0,0,0,0,0,0,
                                            0,0,0,0,0,0,0,0,0,0,0,0,
                                            0,0,0,0,0,0,0,0,0,0,0,0,
                                            0,0,0,0,0,0,0,0,0,0,0,0,
                                            0,0,0,0,0,0,0,0,0,0,0,0};

     int level2[levelWidth * levelHeight] ={1,1,1,1,1,0,0,0,0,0,0,0,
                                            2,2,2,2,2,2,0,0,0,0,0,0,
                                            3,3,3,3,3,3,3,0,0,0,0,0,
                                            4,4,4,4,4,4,4,4,0,0,0,0,
                                            5,5,5,5,5,5,5,5,5,0,0,0,
                                            6,6,6,6,6,6,6,6,6,6,0,0,
                                            1,7,1,7,1,7,1,7,1,7,1,7};

     int blockStateMap[levelWidth * levelHeight] ={1,1,1,1,1,1,1,1,1,1,1,1,
                                                   1,1,1,1,1,1,1,1,1,1,1,1,
                                                   1,1,1,1,1,1,1,1,1,1,1,1,
                                                   1,1,1,1,1,1,1,1,1,1,1,1,
                                                   1,1,1,1,1,1,1,1,1,1,1,1,
                                                   1,1,1,1,1,1,1,1,1,1,1,1,
                                                   1,1,1,1,1,1,1,1,1,1,1,1};


};



#endif
