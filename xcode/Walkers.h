#ifndef Walkers_h
#define Walkers_h

#include "Scene.h"
#include "ImageColorSampler.h"

class Walkers : public Scene {
public:
    static constexpr double POINT_SIZE = 2.0;
    static constexpr double POINT_ALPHA = 0.05;
    static constexpr float POINT_MAX_VEL = 1.0;
    
    static constexpr float NOISE_SCALE = 0.05;
    static constexpr float NOISE_DETAIL = 0.005;
    static constexpr float NOISE_SPEED = 0.05;
    
    Walkers(int width, int height, const fs::path &assetPath, ColorA background)
    : Scene(width, height), noiseScale(NOISE_SCALE), noiseDetail(NOISE_DETAIL), noiseSpeed(NOISE_SPEED) {
        perlin = Perlin();
        perlin.setSeed((int)clock());
        
        sampler = new ImageColorSampler(assetPath);
        sand.setBackground(background);
        
        // define line boundaries
        int n = width;
        int pad = width * 0.04;
        int xMin = pad;
        int xMax = width - pad*2;
        int yMin = height/2.0;
        int yMax = height/2.0;
        
        // generate lines
        double sX = (xMax - xMin) / (double)n;
        double sY = (yMax - yMin) / (double)n;
        double x = xMin;
        double y = yMin;
        
        // TODO: multiple line array
        for(int i = 0; i < 1; ++i) {
            if (sampler) {
                color = sampler->randomSample();
                color.a = POINT_ALPHA;
            }
            particles.reserve(n);
            for(int j = 0; j < n; ++j) {
                Particle p(vec2(x, y), color);
                p.maxVelocity = POINT_MAX_VEL;
                particles.push_back(p);
                x += sX;
                y += sY;
            }
        }
        
    }
    ~Walkers() {
        delete sampler;
    }
    
    void update() override;
    void renderScene(cairo::Context &ctx, int x0, int y0) override;
    
private:
    vector<Particle> particles;
    Perlin perlin;
    ImageColorSampler* sampler = nullptr;
    ColorA color;
    float noiseScale, noiseDetail, noiseSpeed;
};

void Walkers::update() {
    for(auto &particle : particles) {
        particle.velocity.y += noiseScale * perlin.fBm(vec3(particle.position * noiseDetail, app::getElapsedSeconds() * noiseSpeed));
        particle.velocity.x += 0.5 * noiseScale * perlin.fBm(vec3(particle.position * 0.5f * noiseDetail, app::getElapsedSeconds() * 1.2f * noiseSpeed));
        
        // TODO: clamp velocity
        
        particle.update();
    }
}

void Walkers::renderScene(cairo::Context &ctx, int x0, int y0) {
    sand.drawParticles(particles);
    sand.drawFrame(ctx, Rectf(x0, y0, width, height));
}

#endif
