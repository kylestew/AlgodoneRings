#ifndef Walkers_h
#define Walkers_h

#include "Scene.h"
#include "ImageColorSampler.h"

class Walkers : public Scene {
public:
    static const int LINE_COUNT = 9;
    
    static constexpr double POINT_SIZE = 1.0;
    static constexpr double POINT_ALPHA = 0.03;
    static constexpr float POINT_MAX_VEL = 1.0;
    
    static constexpr float NOISE_SCALE = 0.09;
    static constexpr float NOISE_DETAIL = 0.005;
    static constexpr float NOISE_SPEED = 0.05;
    
    Walkers(int width, int height, const fs::path &assetPath, ColorA background)
    : Scene(width, height), noiseScale(NOISE_SCALE), noiseDetail(NOISE_DETAIL), noiseSpeed(NOISE_SPEED) {
        sampler = new ImageColorSampler(assetPath);
        sand.setBackground(background);
        
        // define line boundaries
        int n = width;
        int pad = width * 0.2;
        int xMin = pad;
        int xMax = width - pad;
        int yMin = height/2.0;
        int yMax = height/2.0;
        
        // generate lines
        for(int i = 0; i < LINE_COUNT; ++i) {
            double sX = (xMax - xMin) / (double)n;
            double sY = (yMax - yMin) / (double)n;
            double x = xMin;
            double y = yMin;
            if (sampler) {
                color = sampler->randomSample();
                color.a = POINT_ALPHA;
            }
            vector<Particle> line;
            line.reserve(n);
            for(int j = 0; j < n; ++j) {
                Particle p(vec2(x, y), color);
                p.maxVelocity = POINT_MAX_VEL;
                line.push_back(p);
                x += sX;
                y += sY;
            }
            lines.push_back(line);
        }
        
    }
    ~Walkers() {
        delete sampler;
    }
    
    void update() override;
    void renderScene(cairo::Context &ctx, int x0, int y0) override;
    
private:
    vector<vector<Particle>> lines;
    Perlin perlin;
    ImageColorSampler* sampler = nullptr;
    ColorA color;
    float noiseScale, noiseDetail, noiseSpeed;
};

void Walkers::update() {
    double randomStride = Rand::randFloat();
    double stride = 0;
    for(auto &line : lines) {
        for(auto &particle : line) {
            particle.velocity.y += noiseScale * perlin.fBm(vec3(particle.position * noiseDetail, stride + app::getElapsedSeconds() * noiseSpeed));
            particle.velocity.x += 0.5 * noiseScale * perlin.fBm(vec3(particle.position * 0.5f * noiseDetail, stride + app::getElapsedSeconds() * 1.2f * noiseSpeed));
            particle.update();
        }
        stride += randomStride;
    }
}

void Walkers::renderScene(cairo::Context &ctx, int x0, int y0) {
    for(auto &line : lines) {
        sand.drawParticles(line);
    }
    sand.drawFrame(ctx, Rectf(x0, y0, width, height));
}

#endif
