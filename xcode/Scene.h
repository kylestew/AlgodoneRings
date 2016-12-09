#ifndef Scene_h
#define Scene_h

#include "Utils.h"
#include "Sand.h"
#include "Particle.h"

class Scene {
public:
    Scene(int width, int height, int perlinSeed, ColorA background, ColorA foreground)
    : width(width), height(height), background(background), foreground(foreground) {
        perlin = Perlin();
        perlin.setSeed(perlinSeed);
        
        sand = Sand(width, height);
        sand.setBackground(background);
    }
    
    virtual void update() {}
    virtual void renderScene(cairo::Context &ctx, int x0, int y0) {
        sand.drawFrame(ctx, Rectf(x0, y0, width, height));
    }
protected:
    int width, height;
    Perlin perlin;
    ColorA background, foreground;
    Sand sand;
};

#endif 
