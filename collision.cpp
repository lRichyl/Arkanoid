#include "collision.h"
#include <cmath>
#include <cfloat>

//Collision check using the minkowski difference method
bool DoRectsCollide(Rect b1, Rect b2, V2 *penetration){
	float mdtop   =  b1.y - (b2.y - b2.h);
	float mdbott  =  (b1.y - b1.h) - b2.y;
	float mdleft  =  b1.x - (b2.x + b2.w);
	float mdright =  (b1.x + b1.w) - b2.x;

	if(mdright >= 0 && mdleft <= 0 && mdtop >= 0 && mdbott <= 0){
		// std::cout << "collided" << std::endl;


		float min = FLT_MAX;

		if(std::abs(mdright) < min){
			min = std::abs(mdright);
			penetration->x = mdright;
			penetration->y = 0.f;
		}
		if(std::abs(mdleft) < min){
			min = std::abs(mdleft);
			penetration->x = mdleft;
			penetration->y = 0.f;
		}
		if(std::abs(mdbott) < min){
			min = std::abs(mdbott);
			penetration->x = 0.f;
			penetration->y = mdbott;
		}
		if(std::abs(mdtop) < min){
			min = std::abs(mdtop);
			penetration->x = 0.f;
			penetration->y = mdtop;
		}

		// std::cout << "collision" << std::endl;
		return true;
	}
	penetration->x = 0;
	penetration->y = 0;
	return false;


}

//This only works for axis aligned bounding boxes.
// void Bounce(Rect *boundingBox, V2 *velocity, V2 penetration){
// 	if(penetration.y > 0 || penetration.y < 0) {
//           boundingBox->y -= penetration.y;
//           velocity->y *= -1;
//      }
//
//      if(penetration.x > 0 || penetration.x < 0){
//           boundingBox->x -= penetration.x;
//           velocity->x *= -1;
//      }
// }
