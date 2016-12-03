#ifndef RandomGrids_h
#define RandomGrids_h

#include "Scene.h"

class RandomGrids : public Scene {
public:
    constexpr static const double NOISE = 0.05;
    constexpr static const double SCALE = 0.01;
    
    RandomGrids(int width, int height) : Scene(width, height) {
        // frame in drawing space
        Boundary boundary = getBoundary(width, height, 0.14);
        
        // decide how many grid rows and columns to place
        int side = min(boundary.innerWidth, boundary.innerHeight); // find smallest side
        sideCount = side/40.0;
        if (sideCount < 2) sideCount = 2;
        
        // layout grid
        createGrid(positions, ColorA(0, 0, 0, 0.2), sideCount, side, boundary.xMid, boundary.yMid);
        
        // compute weights and open/closed signs
        for(auto &gp : positions) {
            // TODO: try some perlin for great justiceo
            // TODO: apply olor mapping from sampled image
            
            gp.velocity.x = Rand::randFloat();
            gp.velocity.y = Rand::randFloat() < 0.5 ? -1.0 : 1.0;
        }
    }
    
    void update() override;
    void renderScene(cairo::Context &ctx, int x0, int y0) override;

private:
    int sideCount;
    vector<Particle> positions;
};

void RandomGrids::update() {
        int idx = 0;
        for(auto &gp : positions) {
            int neighbors[4] = {
                idx % sideCount == 0 ? -1 : idx - 1, // start of row?
                idx - sideCount,
                (idx + 1) % sideCount == 0 ? -1 : idx + 1, // end of row?
                idx + sideCount
            };
            
//        // calculate average towards neighbors given weights of neighbors
//        vec2 move = vec2(0, 0);
//        double weight = 0.0;
//        for(auto neighbor : neighbors) {
//            if (neighbor >= 0 && neighbor < positions.size()) {
//                vec2 nMove = positions[neighbor].position;
//                double neighborWeight = positions[neighbor].velocity.x;
//                nMove *= neighborWeight;
//                
//                cout << neighborWeight << " :: " << nMove << endl;
//                
//                weight += neighborWeight;
//                move += nMove;
//            }
//        }
//        cout << weight << " ::  " << move << endl;
//        move /= weight;
//        cout << weight << " ::  " << move << endl;
//        cout << "my pos: " << gp.position << endl;
//        
//        
//        // affect posX, posY or position
//        // some things happen here...
//        vec2 delta = move - gp.position;
//        cout << "delta: " << delta << endl;
//        // ???
//        double dd = sqrt(delta.x*delta.x + delta.y*delta.y || 1.0);
//        delta /= dd;
//        cout << "scaled?: " << delta << endl;
//        
//        double a = Rand::randFloat()*M_PI*2.0;
//        double r = Rand::randFloat()*NOISE;
//        
//        delta *= gp.velocity.y; // sign
//        delta *= SCALE;
//        delta += vec2(cos(a)*r, sin(a)*r);
//        
//        gp.position += delta;
//        
////        gp.position += vec2(dx*SCALE + cos(a)*r, dy*SCALE + sin(a)*r);
//        
////        return {
////        x: limit(gx + sgn*dx*SCALE + Math.cos(a)*r,
////                 boundary.xr-1, boundary.xl),
////        y: limit(gy + sgn*dy*SCALE + Math.sin(a)*r,
////                 boundary.yb-1, boundary.yt)
////        };
//        
//        
//        idx++;
        }
    }

void RandomGrids::renderScene(cairo::Context &ctx, int x0, int y0) {
    sand.drawFrame(ctx, Rectf(x0, y0, width, height));
    
//    int idx = 0;
//    for(auto gp : positions) {
//        // draw lines to all my neighbors
//        int neighbors[4] = {
//            idx % sideCount == 0 ? -1 : idx - 1, // start of row?
//            idx - sideCount,
//            (idx + 1) % sideCount == 0 ? -1 : idx + 1, // end of row?
//            idx + sideCount
//        };
//        for(auto neighbor : neighbors) {
//            if (neighbor >= 0 && neighbor < positions.size()) {
//                // connect with neighbor
//                sand.stroke(4, gp, positions[neighbor]);
//            }
//        }
//        idx++;
//    }
            
    }


#endif 
