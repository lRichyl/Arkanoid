// #include "collision.h"
// #include <iostream>
// #include <cfloat>
// #include <cmath>
// #include <algorithm>
// #include "dynamic_entities.h"
//
// bool CollisionManager::minkowskiDifference(SDL_Rect b1, SDL_Rect b2, Vector2df *penetration){
// 	int mdtop   =  b1.y - (b2.y + b2.h);
// 	int mdbott  =  (b1.y + b1.h) - b2.y;
// 	int mdleft  =  b1.x - (b2.x + b2.w);
// 	int mdright =  (b1.x + b1.w) - b2.x;
//
// 	if(mdright >= 0 && mdleft <= 0 && mdtop <= 0 && mdbott >= 0){
// 		// std::cout << "collided" << std::endl;
//
//
// 		float min = FLT_MAX;
//
// 		if(std::abs(mdright) < min){
// 			min = std::abs(mdright);
// 			penetration->x = mdright;
// 			penetration->y = 0.f;
// 		}
// 		if(std::abs(mdleft) < min){
// 			min = std::abs(mdleft);
// 			penetration->x = mdleft;
// 			penetration->y = 0.f;
// 		}
// 		if(std::abs(mdbott) < min){
// 			min = std::abs(mdbott);
// 			penetration->x = 0.f;
// 			penetration->y = mdbott;
// 		}
// 		if(std::abs(mdtop) < min){
// 			min = std::abs(mdtop);
// 			penetration->x = 0.f;
// 			penetration->y = mdtop;
// 		}
//
// 		// std::cout << "collision" << std::endl;
// 		return true;
// 	}
// 	penetration->x = 0;
// 	penetration->y = 0;
// 	return false;
//
//
// }
//
// void CollisionManager::checkCollisions(CollisionTileMap *collisionLayer){
//
//
// 	for(unsigned int k = 0; k < collisionLayer->dynamicEntities.size(); k++){
// 		if(collisionLayer->dynamicEntities[k] != nullptr){
// 			std::vector<Vector2df> tiles {};
// 			std::vector<Vector2df> distances{};
// 			Vector2df penetrationVector;
// 			distances.resize(9, Vector2df {0, 0});
// 			tiles.resize(9, Vector2df {0, 0});
// 			Entity *e = collisionLayer->dynamicEntities[k];
// 			// std::cout << e->boundingBox.x << " , " << e->boundingBox.x << std::endl;
//
// 			// std::cout << "mario" << std::endl;
// 			// std::cout << collisionLayer->getXTile(e->position.x) << " , " << collisionLayer->getYTile(e->position.y) << std::endl;
// 			tiles[0].x = collisionLayer->getXTile(e->position.x - collisionLayer->tileWidth);
// 			tiles[0].y = collisionLayer->getYTile(e->position.y - collisionLayer->tileHeight);
// 			// std::cout << "Tile0: "<< tile0.x << " , " << tile0.y << std::endl;
//
// 			tiles[1].x = collisionLayer->getXTile(e->position.x);
// 			tiles[1].y = collisionLayer->getYTile(e->position.y - collisionLayer->tileHeight);
// 			// std::cout << "Tile1: "<< tile1.x << " , " << tile1.y << std::endl;
//
// 			tiles[2].x = collisionLayer->getXTile(e->position.x + collisionLayer->tileWidth);
// 			tiles[2].y = collisionLayer->getYTile(e->position.y - collisionLayer->tileHeight);
// 			// std::cout << "Tile2: "<< tile2.x << " , " << tile2.y << std::endl;
//
// 			tiles[3].x = collisionLayer->getXTile(e->position.x - collisionLayer->tileWidth);
// 			tiles[3].y = collisionLayer->getYTile(e->position.y);
// 			// std::cout << "Tile3: "<< tile3.x << " , " << tile3.y << std::endl;
//
// 			tiles[4].x = collisionLayer->getXTile(e->position.x);
// 			tiles[4].y = collisionLayer->getYTile(e->position.y);
// 			// std::cout << "Tile4: "<< tile4.x << " , " << tile4.y << std::endl;
// 			tiles[5].x = collisionLayer->getXTile(e->position.x + collisionLayer->tileWidth);
// 			tiles[5].y = collisionLayer->getYTile(e->position.y);
//
// 			tiles[6].x = collisionLayer->getXTile(e->position.x - collisionLayer->tileWidth);
// 			tiles[6].y = collisionLayer->getYTile(e->position.y + collisionLayer->tileHeight);
// 			// std::cout << "Tile5: "<< tile5.x << " , " << tile5.y << std::endl;
//
// 			tiles[7].x = collisionLayer->getXTile(e->position.x);
// 			tiles[7].y = collisionLayer->getYTile(e->position.y + collisionLayer->tileHeight);
// 			// std::cout << "Tile6: "<< tile6.x << " , " << tile6.y << std::endl;
//
// 			tiles[8].x = collisionLayer->getXTile(e->position.x + collisionLayer->tileWidth);
// 			tiles[8].y = collisionLayer->getYTile(e->position.y + collisionLayer->tileHeight);
// 			// std::cout << "Tile7: "<< tile7.x << " , " << tile7.y << std::endl;
//
// 			distances[0].x = (tiles[0].x * collisionLayer->tileWidth + collisionLayer->tileWidth) - e->position.x;
// 			distances[0].y = (tiles[0].y * collisionLayer->tileHeight + collisionLayer->tileHeight) - e->position.y;
//
// 			distances[1].x = (tiles[1].x * collisionLayer->tileWidth + collisionLayer->tileWidth/2) - e->position.x;
// 			distances[1].y = (tiles[1].y * collisionLayer->tileHeight + collisionLayer->tileHeight) - e->position.y;
//
// 			distances[2].x = (tiles[2].x * collisionLayer->tileWidth) - e->position.x;
// 			distances[2].y = (tiles[2].y * collisionLayer->tileHeight + collisionLayer->tileHeight) - e->position.y;
//
// 			distances[3].x = (tiles[3].x * collisionLayer->tileWidth + collisionLayer->tileWidth) - e->position.x;
// 			distances[3].y = (tiles[3].y * collisionLayer->tileHeight + collisionLayer->tileHeight/2) - e->position.y;
//
// 			distances[4].x = (tiles[4].x * collisionLayer->tileWidth) - e->position.x;
// 			distances[4].y = (tiles[4].y * collisionLayer->tileHeight) - e->position.y;
//
// 			distances[5].x = (tiles[5].x * collisionLayer->tileWidth) - e->position.x;
// 			distances[5].y = (tiles[5].y * collisionLayer->tileHeight + collisionLayer->tileHeight/2) - e->position.y;
//
// 			distances[6].x = (tiles[6].x * collisionLayer->tileWidth + collisionLayer->tileWidth) - e->position.x;
// 			distances[6].y = (tiles[6].y * collisionLayer->tileHeight) - e->position.y;
//
// 			distances[7].x = (tiles[7].x * collisionLayer->tileWidth) - e->position.x;
// 			distances[7].y = (tiles[7].y * collisionLayer->tileHeight) - e->position.y;
// 				// std::cout << distances[i].x << " , " << distances[i].y << std::endl;
//
// 			// std::sort(distances.begin(), distances.end(), [](Vector2df a, Vector2df b){
// 			// });
//
// 			for(unsigned int i = 0; i < distances.size();i++){
// 				Vector2df tempDist;
// 				Vector2df tempTile;
// 				float dist0;
// 				float dist1;
// 				dist0 = sqrt((pow(distances[i].x,2) + pow(distances[i].y,2)));
// 				dist1 = sqrt((pow(distances[i + 1].x,2) + pow(distances[i + 1].y,2)));
// 				if(i < distances.size()  - 1){
// 					if(dist0 > dist1){
// 						tempTile = tiles[i];
// 						tiles[i] = tiles[i + 1];
// 						tiles[i + 1] = tempTile;
// 						tempDist = distances[i];
// 						distances[i] = distances[i + 1];
// 						distances[i + 1] = tempDist;
// 					}
// 				}
// 			}
//
//
//
// 			//Checking collisions against static blocks
// 			for(unsigned int i = 0; i < tiles.size();i++){
// 				Entity *collidedStaticEntity = collisionLayer->entities[(int)tiles[i].x][(int)tiles[i].y];
// 				if(tiles[i].x > 0 && tiles[i].y > 0){//We do this so that we don't check collisions outside the world
// 					if( collidedStaticEntity != nullptr){
// 						if(CollisionManager::minkowskiDifference(e->boundingBox,collidedStaticEntity->boundingBox, &penetrationVector)){
// 							// std::cout << &penetrationVector << std::endl;
// 							e->onStaticEntityCollision(penetrationVector, collidedStaticEntity);
// 						}
// 					}
// 				}
// 			}
//
//
// 			//The dynamic entities should also be sorted by the distance they have to the player
// 			for(unsigned int i = 0; i < collisionLayer->dynamicEntities.size();i++){
// 				Vector2df penetrationVector;
// 				Entity *collidedDynamicEntity = collisionLayer->dynamicEntities[i];
// 				if(collidedDynamicEntity->ID != e->ID && collidedDynamicEntity->isActive){
// 					if(CollisionManager::minkowskiDifference(e->boundingBox,collidedDynamicEntity->boundingBox, &penetrationVector)){
// 						e->onDynamicEntityCollision(penetrationVector, collidedDynamicEntity);
//
// 					}
// 				}
// 			}
//
// 		}
// 	}
// }
