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

     void DrawLevel();
     void DrawBall();

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
     static constexpr V2 blockSize = {63, 32};


     static constexpr int level1[levelWidth * levelHeight] ={0,0,0,0,0,0,0,0,0,0,0,0,
                                     1,1,1,1,1,1,1,1,1,1,1,1,
                                     2,2,2,2,2,2,2,2,2,2,2,2,
                                     3,3,3,3,3,3,3,3,3,3,3,3,
                                     4,4,4,4,4,4,4,4,4,4,4,4,
                                     5,5,5,5,5,5,5,5,5,5,5,5,
                                     6,6,6,6,6,6,6,6,6,6,6,6};

};



#endif
