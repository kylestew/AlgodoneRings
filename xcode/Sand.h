/*
 
 Sand: grainy drawing library for Cinder Surfaces
 Applies a random grain to lines drawn between specified particles
 Allows for multiple compositing modes
 
 */
#ifndef Sand_h
#define Sand_h

#include "cinder/cairo/Cairo.h"
#include "cinder/Perlin.h"
#include "cinder/Rand.h"

using namespace ci;
using namespace std;

#include "Particle.h"

class Sand {
public:
    Sand() {}
    Sand(int width, int height) : background{ColorA(1,1,1,1)}, grain{1.0} {
        offscreenBuffer = cairo::SurfaceImage(width, height, false);
        offscreenContext = cairo::Context(offscreenBuffer);
        clear();
        perlin.setSeed((int)clock());
    }
    
    void setBackground(ColorA col) { background = col; clear(); }
    void setGrain(double aGrain) {
        // grain is scaled up for surface
        // ex: 4.0 = a particle can randomly move up to 4% from its original position
//        grain = width * (aGrain/200.0);
        grain = aGrain;
    }
    
    void clear() {
        offscreenContext.setSource(background);
        offscreenContext.paint();
    }
    
    void drawParticles(vector<Particle> particles) {
        for(auto &particle : particles) {
            drawPoint(particle.position, particle.color, 1);
        }
    }
    
    void drawPoints(vector<Particle> particles, double size) {
        for(auto &particle : particles) {
            drawPoint(particle.position, particle.color, size);
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
            float rnd = Rand::randFloat();
            drawPoint(vec2(p0.position.x + rnd*del.x, p0.position.y + rnd*del.y), p0.color, 1);
        }
    }
    
    void drawEdges(vector<Particle> points, double lineWidth, ColorA color) {
        if (points.size() == 0)
            return;
        
        vec2 p0 = points[0].position;
        offscreenContext.moveTo(p0);
        for(int i = 1; i < points.size(); ++i) {
            offscreenContext.lineTo(points[i].position);
        }
        offscreenContext.closePath();
        
        offscreenContext.setLineWidth(lineWidth);
        offscreenContext.setSource(color);
        offscreenContext.stroke();
    }
    
    void drawFrame(cairo::Context &ctx, Rectf dest) {
        ctx.copySurface(offscreenBuffer, offscreenBuffer.getBounds(), dest);
    }
    
private:
    cairo::SurfaceImage     offscreenBuffer;
    cairo::Context          offscreenContext;
    
    Perlin perlin;
//    int width, height;
    ColorA background;
    double grain;
    
    void drawPoint(vec2 point, ColorA color, double size) {
        // apply grain as perlin brownian noise
        // TODO: where does grain need to be applied?
//        point += grain * perlin.fBm(point.x*0.1f, point.y*0.1f, app::getElapsedSeconds() * 0.1f);
        
        offscreenContext.setSource(color);
        offscreenContext.rectangle(point.x, point.y, size, size);
        offscreenContext.fill();
    }
    
    /*
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
     */
    
};

#endif /* Sand_h */
