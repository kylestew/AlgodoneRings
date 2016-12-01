/*
 
 Sand: grainy drawing library for Cinder Surfaces
 Applies a random grain to lines drawn between specified particles
 Allows for multiple compositing modes
 
 */
#ifndef Sand_h
#define Sand_h

#include "cinder/Perlin.h"
#include "cinder/Rand.h"

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
//        grain = width * (aGrain/200.0);
        grain = aGrain;
    }
    
    void drawParticles(vector<Particle> particles) {
        for(auto &particle : particles) {
            drawPoint(particle.position, particle.color);
        }
    }
    
    void connectParticles(vector<Particle> particles) {
//        if (particles.size()<2)
//            return;
//        
//        for(int i = 1; i < particles.size(); ++i) {
//            Particle& p0 = particles[i-1];
//            Particle& p1 = particles[i];
//            drawLine(p0.scalePosition(width, height), p1.scalePosition(width, height), p0.color, p1.color);
//        }
    }
    
    void stroke(int detail, Particle p0, Particle p1) {
        vec2 del = p1.position - p0.position;
        
        for(int i = 0; i < detail; i++) {
//            float rnd = Rand::randFloat();
//            drawPoint(vec2(p0.position.x + rnd*del.x, p0.position.y + rnd*del.y), p0.color);
            
            float rnd = (float)i/(float)detail;
            drawPoint(vec2(p0.position.x + rnd*del.x, p0.position.y + rnd*del.y), p0.color);
        }
    }
    
    void display(int scaleDown = 1) {
        Rectf drawRect(0, 0, width/scaleDown, height/scaleDown);
        gl::draw(gl::Texture2d::create(surface), drawRect);
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
        c.r += alpha * (col.r + c.r*invA);
        c.g += alpha * (col.g + c.g*invA);
        c.b += alpha * (col.b + c.b*invA);
        c.a = alpha + c.a*invA;
        
        surface.setPixel(pos, c);
    }
    
    void drawPoint(vec2 point, ColorA color) {
        // apply grain as perlin brownian noise
        // TODO: where does grain need to be applied?
//        point += grain * perlin.fBm(point.x*0.1f, point.y*0.1f, app::getElapsedSeconds() * 0.1f);
        blendOver(point, color);
    }
    
    void drawLine(vec2 p0, vec2 p1, ColorA c0, ColorA c1) {
        double x0 = p0.x;
        double y0 = p0.y;
        double x1 = p1.x;
        double y1 = p1.y;
        
        bool steep = false;
        // if the line is steep, we transpose drawing
        if (std::abs(x0-x1)<std::abs(y0-y1)) {
            std::swap(x0, y0);
            std::swap(x1, y1);
            steep = true;
        }
        if (x0>x1) { // draw left-to-right always
            std::swap(x0, x1);
            std::swap(y0, y1);
        }
        
        for (int x=x0; x<=x1; x += 2) {
            double t = (x-x0)/(double)(x1-x0);
            int y = y0*(1.-t) + y1*t;
            ColorA c = lerp(c0, c1, t);
            
            if (steep) {
                drawPoint(vec2(y, x), c);
            } else {
                drawPoint(vec2(x, y), c);
            }
        }
    }
    
};

#endif /* Sand_h */
