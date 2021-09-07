#ifndef GAME_H
#define GAME_H
#include "renderer.h"
#include "entities.h"
#include "math.h"
#include <vector>

struct Game{
     void UpdateGame(float dt, Renderer *renderer);
     void DrawGame(Renderer *renderer, Window *window);
     void GameLoop(float dt, Renderer *renderer, Window *window);
     Game();

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
     Block blackBlock;

};



#endif
