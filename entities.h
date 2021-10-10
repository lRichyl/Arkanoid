#ifndef PADDLE_H
#define PADDLE_H
#include "renderer.h"
#include "math.h"
#include <map>

enum Blocks{
     BLOCKS_EMPTY,
     BLOCKS_BLACK, // Black blocks are indestructible
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

     void SetPosition(V2 position){
          boundingBox.x = position.x;
          boundingBox.y = position.y;
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

enum PowerUpType{
     POWER_NONE = 0x0,
     POWER_LASER = 0x1,
     POWER_ENLARGE = 0x2,
     POWER_CATCH = 0x4,
     POWER_SLOW = 0x8,
     POWER_DISRUPTION = 0x10,
     POWER_EXTRA_PLAYER = 0x20,
     POWER_COUNT = 6
};

struct PowerUp : public Entity{
     void Init(V2 position, Rect clippingBoxA, Texture textureA, PowerUpType typeA){
          texture = textureA;
          boundingBox = {position.x, position.y, 32, 16};
          clippingBox = clippingBoxA;
          type = typeA;
     }
     void Update(float dt);

     PowerUpType type = PowerUpType::POWER_NONE;
     float gravity = -80.0f;
};

struct Paddle : public Entity{
     void Init(V2 position, Texture t, Renderer *renderer){
          boundingBox = {position.x, position.y, 64, 16};
          clippingBox = {0 , 0, 32 , 8};
          texture = t;
          // powerupFlags |= PowerUpType::POWER_DISRUPTION | PowerUps::POWER_EXTRA_PLAYER;
          // printf("%x\n", powerupFlags);

          //All shaders should be compiled at the same time in the same place.
          //This is temporary and will be moved to its own system.
          shader.name = "Player Shader";
          compile_shader_program(&shader, "assets/shaders/player_vertex_shader.txt", "assets/shaders/player_fragment_shader.txt");
          initialize_texture_sampler(shader);
          load_mvp_to_shader(renderer, shader);
     }

     void Update(float dt, Renderer *renderer);
     // void Draw(Renderer *renderer){
     //      render_quad(renderer, &boundingBox, &texture, &clippingBox);
     // }
     void ResetPosition(Window *window);
     // void Draw(Renderer *renderer);

     V2 direction = {};
     float speed = 300;
     ShaderProgram shader;
     // int powerUpFlags = 0;
     // Texture texture;
};

struct Ball : public Entity{
     void Init(V2 position, Texture t){
          boundingBox = {position.x, position.y, 16, 12};
          clippingBox = {9 , 9, 8 , 7};
          texture = t;
          // velocity.y = speed;
     }
     void Update(float dt, Renderer *renderer, Paddle *paddle);
     void ResetPosition(Paddle *paddle);
     void Bounce(V2 penetration);
     // void Draw(Renderer *renderer){
     //      render_quad(renderer, &boundingBox, &texture, &clippingBox, false, 255, V3 {1.0f,0.0f,0.0f});
     // }
     // Texture = texture;
     BallState state = ON_PADDLE;
     float speed = 200;
     V2 velocity = {0, 0};

};

struct Laser : public Entity{
     void Init(V2 position,Texture t){
          boundingBox = {position.x, position.y, 8, 14};
          clippingBox = {224 , 15, 8 , 14};
          texture = t;
     }

     void Update(float dt);

     float speed = 400;
};


#endif
