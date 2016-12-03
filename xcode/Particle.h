#ifndef Particle_h
#define Particle_h

using namespace ci;
using namespace ci::app;

class Particle {
public:
    Particle(const vec2 &position, ColorA color)
        : position(position), velocity(vec2(0)), maxVelocity(0), color(color) {}
    
    vec2 position;
    vec2 velocity;
    double maxVelocity;
    ColorA color;
    
    void update() {
        if (maxVelocity > 0)
            assert(false); // not clamping
            velocity = clamp(velocity, vec2(0, 0), vec2(maxVelocity, maxVelocity));
        position += velocity;
    }
private:
};

#endif 
