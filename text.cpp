#include "text.h"
#include "renderer.h"

 std::map<std::string, Rect> Text::glyphsMap = {{"A", Rect {15, 49, 16, 16}},
{"B", Rect {31, 49, 16, 16}},
{"C", Rect {47, 49, 16, 16}},
{"D", Rect {63, 49, 16, 16}},
{"E", Rect {79, 49, 16, 16}},
{"F", Rect {95, 49, 16, 16}},
{"G", Rect {111, 49, 16, 16}},
{"H", Rect {127, 49, 16, 16}},
{"I", Rect {143, 49, 16, 16}},
{"J", Rect {159, 49, 16, 16}},
{"K", Rect {175, 49, 16, 16}},
{"L", Rect {191, 49, 16, 16}},
{"M", Rect {207, 49, 16, 16}},
{"N", Rect {223, 49, 16, 16}},
{"O", Rect {239, 49, 16, 16}},
{"P", Rect {0, 33, 16, 16}},
{"Q", Rect {15, 33, 16, 16}},
{"R", Rect {31, 33, 16, 16}},
{"S", Rect {47, 33, 16, 16}},
{"T", Rect {65, 33, 14, 16}},
{"U", Rect {79, 33, 16, 16}},
{"V", Rect {95, 33, 16, 16}},
{"W", Rect {111, 33, 16, 16}},
{"X", Rect {127, 33, 16, 16}},
{"Y", Rect {145, 33, 14, 16}},
{"Z", Rect {159, 33, 16, 16}},
{"0", Rect { 0, 65, 16, 16}},
{"1", Rect { 17, 65, 14, 16}},
{"2", Rect { 31, 65, 16, 16}},
{"3", Rect { 47, 65, 16, 16}},
{"4", Rect { 63, 65, 16, 16}},
{"5", Rect { 79, 65, 16, 16}},
{"6", Rect { 95, 65, 16, 16}},
{"7", Rect { 111, 65, 16, 16}},
{"8", Rect { 127, 65, 16, 16}},
{"9", Rect { 143, 65, 16, 16}},
{".", Rect { 222, 80, 16, 16}},
{"a", Rect {15, 17, 16, 16}},
{"b", Rect {32, 17, 16, 16}},
{"c", Rect {48, 17, 16, 16}},
{"d", Rect {64, 17, 16, 16}},
{"e", Rect {80, 17, 16, 16}},
{"f", Rect {96, 17, 16, 16}},
{"g", Rect {112, 17, 16, 16}},
{"h", Rect {128, 17, 16, 16}},
{"i", Rect {144, 17, 16, 16}},
{"j", Rect {160, 16, 16, 16}},
{"k", Rect {176, 17, 16, 16}},
{"l", Rect {194, 17, 16, 16}},
{"m", Rect {208, 17, 16, 16}},
{"n", Rect {224, 17, 16, 16}},
{"o", Rect {240, 17, 16, 15}},
{"p", Rect {0, 0, 16, 16}},
{"q", Rect {16, 1, 16, 16}},
{"r", Rect {32, 1, 16, 16}},
{"s", Rect {48, 1, 16, 16}},
{"t", Rect {64, 1, 14, 16}},
{"u", Rect {80, 1, 16, 16}},
{"v", Rect {96, 1, 16, 16}},
{"w", Rect {113, 1, 16, 16}},
{"x", Rect {129, 1, 16, 16}},
{"y", Rect {144, 1, 14, 16}},
{"z", Rect {160, 1, 16, 16}}
};

void Text::Render(Renderer *renderer, std::string text){
     if(currentText != text){
          currentText = text;
          glyphsToRender.clear();
          glyphsPositions.clear();
          SetGlyphs();
     }
     for(unsigned int i = 0;i < glyphsToRender.size(); i++){
          // SDL_RenderCopy(renderer, glyphsMap->texture, &glyphsToRender[i], &glyphsPositions[i]);
          render_quad(renderer, &glyphsPositions[i], &font, &glyphsToRender[i]);
     }
}
void Text::SetGlyphs(){
     for(unsigned int i = 0; i < currentText.size();i++ ){
          std::string t = currentText.substr(i,1);
          glyphsToRender.push_back(glyphsMap[t]);
          float xFinalPos = (i * size) + x;
          // if(t == "i") x -= 10;
          // if(t == "l") x -= 12;
          // if(currentText[i + 1] == 'i') x -= 10;
          // if(currentText[i + 1] == 'l') x -= 7;
          float yFinalPos = y;
          glyphsPositions.push_back(Rect {xFinalPos, yFinalPos, size, size});
     }
}
