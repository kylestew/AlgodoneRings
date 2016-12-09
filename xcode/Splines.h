#ifndef Splines_h
#define Splines_h

#include "Scene.h"

class Splines : public Scene {
public:
    Splines(int width, int height, ColorA background)
    : Scene(width, height) {
        
//        https://github.com/inconvergent/inconvergent.github.io/blob/master/spurious-splines/src/draw.js#L42
//        http://stackoverflow.com/questions/7054272/how-to-draw-smooth-curve-through-n-points-using-javascript-html5-canvas
        
    }
};

#endif
