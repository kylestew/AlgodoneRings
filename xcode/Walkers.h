#ifndef Walkers_h
#define Walkers_h

using namespace ci;

#include "Particle.h"

class Walkers {
public:
    Walkers(int count) {
        particles.reserve(count);
        for (int i = 0; i < count; ++i) {
            float x = lmap((float)i, 0.0f, (float)count, 0.0f, 1.0f);
            particles.push_back(Particle(vec2(x, 0.5)));
        }
    }
    
    void update(float randomScale) {
        for(auto &particle : particles) {
            particle.velocity = vec2(0, (1.0-2.0*Rand::randFloat()) * randomScale);
            particle.update();
        }
    }
    
    vector<Particle> particles;
private:
};

#endif /* Walkers_h */
