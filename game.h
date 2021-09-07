#ifndef GAME_H
#define GAME_H
#include "renderer.h"
#include "entities.h"
#include "math.h"

struct Game{
     void UpdateGame(float dt, Renderer *renderer);
     void DrawGame(Renderer *renderer, Window *window);
     void GameLoop(float dt, Renderer *renderer, Window *window);
     Game();

     Paddle paddle;
     
};



#endif
