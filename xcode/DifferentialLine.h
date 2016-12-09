#ifndef DifferentialLine_h
#define DifferentialLine_h

#import "Scene.h"
#import "Differential.h"

class DifferentailLine : public Scene {
public:
    
    DifferentailLine(int width, int height, int seed, ColorA background, ColorA foreground)
    : Scene(width, height, seed, background, foreground) {
        
        // frame in drawing space -- push in pretty far as the wandering will blow outwards
        Boundary boundary = getBoundary(width, height, 0.25);
        
        
        int num = 10;
//        const dotSize = 5;
        int rad = 20;
        int near = 40;
        int far = 100;
        int maxNum = 150;
        
//        const stp = 0.3;
//        const attract = 0.1*stp;
//        const reject = 0.01*stp;
//        const splitCandidate = near;
//        const splitObligatory = near*4;
//        const splitProb = 0.005;
        
        
        
        
        vector<DifferentialPoint> points = Differential::getConnectedCircle(num, boundary.xMid, boundary.yMid, rad);
        
        differential = Differential(points, maxNum, near, far);
    }
    
    void update() override;
    void renderScene(cairo::Context &ctx, int x0, int y0) override;
    
private:
    Differential differential;
};

void DifferentailLine::update() {
    // step Differntial class object
//    const alive = D.step(
//                         attract,
//                         reject,
//                         splitCandidate,
//                         splitObligatory,
//                         splitProb,
//                         false
//                         );
}

void DifferentailLine::renderScene(cairo::Context &ctx, int x0, int y0) {
//    ctx.setSource(foreground);
//    int idx = 0;
//    for(auto gp : positions) {
//        // draw lines to all my neighbors
//        vector<int> neighbors = connections[idx];
//        for(auto neighbor : neighbors) {
//            // connect with neighbor
//            sand.stroke(4, gp, positions[neighbor]);
//        }
//        idx++;
//    }
    
    sand.clear();
    
    
    // DRAW
    
    
    
    // draw visual line connectors - debugging
//    ctx.lineWidth = 2;
//    ctx.strokeStyle = CYAN;
//    D.points.forEach((p) => {
//        if (p.nearest) {
//            p.nearest.forEach((pn) => {
//                ctx.beginPath();
//                ctx.moveTo(p.x, p.y);
//                ctx.lineTo(pn.x, pn.y);
//                ctx.stroke();
//            });
//        }
//    });
    
    
    
    // draw boundary (highlight dots)
    vector<Particle> particles;
    for(auto point : differential.points) {
        particles.push_back(Particle(point.pos, foreground));
        assert(false); // these are actually not in order dude
    }
    sand.drawEdges(particles, 4.0, foreground);
    
    
//    ctx.lineWidth = 5;
//    ctx.strokeStyle = GRAY;
//    drawEdges(ctx, D.points);
//    D.points.forEach((p) => {
//        const x = p.x;
//        const y = p.y;
//        ctx.beginPath();
//        if (p.age < 60) {
//            ctx.arc(x, y, 1.5*dotSize, 0, TWOPI);
//        } else {
//            ctx.arc(x, y, dotSize, 0, TWOPI);
//        }
//        ctx.fill();
//    });
    
    
    sand.drawFrame(ctx, Rectf(x0, y0, width, height));
}

#endif
