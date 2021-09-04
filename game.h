#ifndef GAME_H
#define GAME_H
#include "renderer.h"

struct Game{
     Texture smiley = make_texture("assets/textures/smiley.png");
     void UpdateGame(float dt, Renderer *renderer);
     void DrawGame(Renderer *renderer, Window *window);
     void GameLoop(float dt, Renderer *renderer, Window *window);

};

#endif
