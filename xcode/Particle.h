#ifndef Particle_h
#define Particle_h

/* Particles are in [0,1) space */
class Particle {
public:
    Particle(const vec2 &position, ColorA color)
        : position(position), velocity(vec2(0)), color(color) {}
    
    vec2 position;
    vec2 velocity;
    ColorA color;
    
    vec2 scalePosition(float width, float height) {
        return vec2(position.x * width, position.y * height);
    }
    
    void update() {
        position += velocity;
    }
private:
};

#endif /* Particle_h */
