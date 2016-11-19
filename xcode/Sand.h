/*
 
 Sand: grainy drawing library for Cinder Surfaces
 Applies a random grain to lines drawn between specified particles
 Allows for multiple compositing modes
 
 */
#ifndef Sand_h
#define Sand_h

#include "cinder/Perlin.h"
using namespace ci;
using namespace std;

#include "Particle.h"

class Sand {
public:
    Sand() {}
    Sand(int width, int height) : width{width}, height{height}, alpha{0}, grain{1.0} {
        surface = Surface32f(width, height, false);
        perlin.setSeed((int)clock());
    }
    
    void setBackground(ColorA col) { background = col; clear(); }
    void setAlpha(float aAlpha) { alpha = aAlpha; }
    void setGrain(float aGrain) {
        // grain is scaled up for surface
        // ex: 4.0 = a particle can randomly move up to 4% from its original position
        grain = width * (aGrain/200.0);
    }
    
    void drawParticles(vector<Particle> particles) {
        for(auto &particle : particles) {
            blendOver(applyGrain(particle), particle.color);
        }
    }
    
    void connectParticles(vector<Particle> particles) {
        if (particles.size()<2)
            return;
        
        for(int i=1; i<particles.size(); ++i) {
            Particle& p0 = particles[i-1];
            Particle& p1 = particles[i];
            drawLine(applyGrain(p0), applyGrain(p1), p0.color, p1.color);
        }
    }
    
    void display() {
        gl::draw(gl::Texture2d::create(surface));
    }
    
private:
    Perlin perlin;
    int width, height;
    Surface32f surface;
    ColorA background;
    float alpha;
    float grain;
    
    void clear() {
        Area area(0, 0, surface.getWidth(), surface.getHeight());
        Surface32f::Iter iter = surface.getIter(area);
        while(iter.line()) {
            while(iter.pixel()) {
                iter.r() = background.r;
                iter.g() = background.g;
                iter.b() = background.b;
                iter.a() = background.a;
            }
        }
    }
    
    void blendOver(vec2 pos, ColorA col) {
        ColorA c = surface.getPixel(pos);
        
        float invA = 1.0 - alpha;
        c.r = col.r + c.r*invA;
        c.g = col.g + c.g*invA;
        c.b = col.b + c.b*invA;
        c.a = alpha + c.a*invA;
        
        surface.setPixel(pos, c);
    }
    
    void drawLine(vec2 p0, vec2 p1, ColorA c0, ColorA c1) {
        // TODO: lerp colors
    }
    
    vec2 applyGrain(Particle& p) {
        // grain = random offset of pixel
        vec2 pos = p.scalePosition(width, height);
        pos += grain * perlin.dfBm(pos.x + clock(), pos.y + clock());
        return pos;
    }
    
};

#endif /* Sand_h */
