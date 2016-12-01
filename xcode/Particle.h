#ifndef Particle_h
#define Particle_h

using namespace ci;
using namespace ci::app;

class Particle {
public:
    Particle(const vec2 &position, ColorA color)
        : position(position), velocity(vec2(0)), color(color) {}
    
    vec2 position;
    vec2 velocity;
    ColorA color;
    
    void update() {
        position += velocity;
    }
private:
};

#endif 
