#include "renderer.h"
#include "game.h"

float v = 100.0f;
float pos_x = 0;
void Game::UpdateGame(float dt, Renderer *renderer){


}
Rect position = {100,100,20,20};
Rect clip = {0 , 365, 360 , 365};
void Game::DrawGame(Renderer *renderer, Window *window){
     //Right now we only draw the main batch, which can have 1000 quads with as many textures units as the
     //system can handle.
     //TODO: Add more batches to draw on when we run out of texture units or surpass 1000 quads.
     static Texture smiley = make_texture("assets/textures/smile.png");
     render_quad(renderer, &position, &smiley, &clip);
     renderer_draw(renderer);

     swap_buffers(window);
     poll_events();
}

void Game::GameLoop(float dt, Renderer *renderer, Window *window){
     UpdateGame(dt, renderer);
     DrawGame(renderer, window);
}
