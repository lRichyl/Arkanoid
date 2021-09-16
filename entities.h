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

enum BallState{
     ON_PADDLE,
     MOVING
};

struct Entity{
     Rect boundingBox = {};
     Rect clippingBox = {};
     Texture texture;

     void Draw(Renderer *renderer){
          render_quad(renderer, &boundingBox, &texture, &clippingBox);
     }
};

// struct Block : public Entity{
//
//      void Init(V2 bBox, Rect clipBox, Texture t){
//           boundingBox = {bBox.x, bBox.y, size.x, size.y};
//           clippingBox = clipBox;
//           texture = t;
//      }
//
//
//      Texture texture;
// };
struct Paddle : public Entity{
     void Init(V2 position, Texture t){
          boundingBox = {position.x, position.y, 64, 16};
          clippingBox = {0 , 0, 32 , 8};
          texture = t;
     }

     void Update(float dt, Renderer *renderer);
     // void Draw(Renderer *renderer);

     V2 direction = {};
     float speed = 300;
     // Texture texture;
};

struct Ball : public Entity{
     void Init(V2 position, Texture t){
          boundingBox = {position.x, position.y, 16, 12};
          clippingBox = {9 , 9, 8 , 6};
          texture = t;
          velocity.y = speed;
     }
     void Update(float dt, Renderer *renderer, Paddle *paddle);
     void ResetPosition(Paddle *paddle);
     void Bounce(V2 penetration);
     // Texture = texture;
     BallState state = ON_PADDLE;
     float speed = 200;
     V2 velocity = {0, 0};
};


#endif
