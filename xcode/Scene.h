#ifndef Scene_h
#define Scene_h

#include "Utils.h"
#include "Sand.h"
#include "Particle.h"

class Scene {
public:
    Scene(int width, int height) : width(width), height(height) {
        sand = Sand(width, height);
    }
    
    static constexpr const char* SCENE_NAME = "SCENE NAME";
    
    virtual void update() {}
    virtual void renderScene(cairo::Context &ctx, int x0, int y0) {}
protected:
    int width, height;
    Sand sand;
};

#endif 
