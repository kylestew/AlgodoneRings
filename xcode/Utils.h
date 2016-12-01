#ifndef Utils_h
#define Utils_h

#include "Particle.h"

using namespace ci;
using namespace ci::app;

struct Boundary {
    int xMin, xMax, yMin, yMax;
    int xMid, yMid;
    int innerWidth, innerHeight;
};

Boundary getBoundary(int width, int height, float paddingPercentage) {
    Boundary bound;
    
    // push in based on padding
    bound.xMin = width*paddingPercentage;
    bound.xMax = width - bound.xMin;
    bound.yMin = height*paddingPercentage;
    bound.yMax = height - bound.yMin;
    
    // calculate mid points and new frame
    bound.innerWidth = bound.xMax - bound.xMin;
    bound.innerHeight = bound.yMax - bound.yMin;
    bound.xMid = bound.xMin + bound.innerWidth * 0.5;
    bound.yMid = bound.yMin + bound.innerHeight * 0.5;
    
    return bound;
}

void createGrid(std::vector<Particle>& positions, ColorA color, int count, int length, int xCenter, int yCenter) {
    int spacing = length/(count-1); // 1 - count = number of connecting spaces
    int xStart = xCenter - length/2.0;
    int yStart = yCenter - length/2.0;
    for (int col = 0; col < count; col++) {
        for (int row = 0; row < count; row++) {
            int x = xStart + spacing * row;
            int y = yStart + spacing * col;
            positions.push_back(Particle(vec2(x, y), color));
        }
    }
}

#endif
