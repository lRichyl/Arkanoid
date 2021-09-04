#include "math.h"

void V2::SwitchXComponents(V2 *a, V2 *b){
     float temporalX = a->x;

     a->x = b->x;
     b->x = temporalX;
}
