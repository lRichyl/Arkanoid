#ifndef TEXT_H
#define TEXT_H
#include "renderer.h"
#include "math.h"
#include <vector>
#include <map>
#include <string>

struct Text{
     void SetGlyphs();
     void Render(Renderer *renderer, std::string text);
     Text(Texture font, float size, V2 position){
          this->font = font;
          this->size = size;
          x = position.x;
          y = position.y;
     }

     int x;
     int y;
     float size;
     std::string currentText;
     Texture font;

     std::vector<Rect> glyphsToRender {};
     std::vector<Rect> glyphsPositions {};

     static std::map<std::string, Rect> glyphsMap;

};

#endif
