#ifndef PADDLE_H
#define PADDLE_H
#include "renderer.h"
#include "math.h"

struct Entity{
     Rect boundingBox = {};
     Rect clippingBox = {};
};

struct Paddle : public Entity{
     void Init(V2 position){
          boundingBox = {position.x,position.y,64,16};
          clippingBox = {0 , 0, 32 , 8};
     }

     void Update(float dt, Renderer *renderer);
     void Draw(Renderer *renderer);

     V2 direction = {};
     float speed = 300;
     Texture arkanoidBlocks = make_texture("assets/textures/Arkanoid blocks.png");
};

#endif
