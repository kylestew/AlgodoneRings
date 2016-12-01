#ifndef Walkers_h
#define Walkers_h

#include "cinder/Rand.h"
#include "cinder/Perlin.h"

#include "Particle.h"
#include "ImageColorSampler.h"

using namespace ci;

class Walkers {
public:
    Walkers() : color(ColorA(0,0,0,1)) {
        perlin = Perlin();
    }
    ~Walkers() {
        delete sampler;
    }
    
    void setColor(ColorA color_) { color = color_; };
    void setNoiseScale(float scale) { noiseScale = scale; };
    void setNoiseDetail(float detail) { noiseDetail = detail; };
    void setNoiseSpeed(float speed) { noiseSpeed = speed; };
    void setImageToSample(const fs::path &assetPath) {
        sampler = new ImageColorSampler(assetPath);
    }
    
    // == POINT INITIALIZERS ==
    void createLine(int n, double xMin, double xMax, double yMin, double yMax) {
        double sX = (xMax - xMin) / (double)n;
        double sY = (yMax - yMin) / (double)n;
        double x = xMin;
        double y = yMin;
        particles.reserve(n);
        for(int i = 0; i < n; ++i) {
            if (sampler)
                particles.push_back(Particle(vec2(x, y), sampler->perlinSample(vec2(x, y))));
            else
                particles.push_back(Particle(vec2(x, y), color));
            x += sX;
            y += sY;
        }
    }
    
    void update(float noiseScale, float noiseDetail, float noiseSpeed) {
        for(auto &particle : particles) {
            
            particle.velocity.y += noiseScale * perlin.fBm(vec3(particle.position * noiseDetail, app::getElapsedSeconds() * noiseSpeed));
            
            particle.velocity.x += 0.5 * noiseScale * perlin.fBm(vec3(particle.position * 0.5f * noiseDetail, app::getElapsedSeconds() * 1.2f * noiseSpeed));
            
            particle.update();
        }
    }
    
    vector<Particle> particles;
private:
    Perlin perlin;
    ImageColorSampler* sampler = nullptr;
    ColorA color;
    float noiseScale, noiseDetail, noiseSpeed;
};

#endif /* Walkers_h */
