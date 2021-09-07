#ifndef PADDLE_H
#define PADDLE_H
#include "renderer.h"
#include "math.h"

enum Blocks{
     BLOCKS_BLACK,
     BLOCKS_PINK,
     BLOCKS_PURPLE,
     BLOCKS_BLUE,
     BLOCKS_GREEN,
     BLOCKS_ORANGE,
     BLOCKS_RED,
     BLOCKS_COUNT
};

struct Entity{
     Rect boundingBox = {};
     Rect clippingBox = {};
};

struct Block : public Entity{

     void Init(V2 bBox, Rect clipBox, Texture t){
          boundingBox = {bBox.x, bBox.y, size.x, size.y};
          clippingBox = clipBox;
          texture = t;
     }

     void Update(float dt, Renderer *renderer);
     void Draw(Renderer *renderer);

     const V2 size = {48, 16};
     Texture texture;
};

struct Paddle : public Entity{
     void Init(V2 position, Texture t){
          boundingBox = {position.x, position.y, 64, 16};
          clippingBox = {0 , 0, 32 , 8};
          texture = t;
     }

     void Update(float dt, Renderer *renderer);
     void Draw(Renderer *renderer);

     V2 direction = {};
     float speed = 300;
     Texture texture;
};

#endif