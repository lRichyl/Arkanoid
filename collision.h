// For collisions remmember to do all your calculations with respect
// to the last state of the bounding box.
#ifndef COLLISION_H
#define COLLISION_H
#include "renderer.h"
#include "math.h"


bool DoRectsCollide(Rect b1, Rect b2, V2 *penetration);
// void Bounce(Rect *boundingBox, V2 *velocity, V2 penetration);

#endif
