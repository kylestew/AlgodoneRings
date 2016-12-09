#ifndef ConnectedPoints_h
#define ConnectedPoints_h

#include "Scene.h"

class ConnectedPoints : public Scene {
public:
    constexpr static const double NOISE = 0.025;
    constexpr static const double SCALE = 0.005;
    
    constexpr static const double GRID_TARGET_COUNT = 12;
    
    ConnectedPoints(int width, int height, int seed, ColorA background, ColorA foreground)
    : Scene(width, height, seed, background, foreground) {
        // frame in drawing space -- push in pretty far as the wandering will blow outwards
        Boundary boundary = getBoundary(width, height, 0.25);
        
        
        
        // decide how many grid rows and columns to place
        int side = min(boundary.innerWidth, boundary.innerHeight); // find smallest side
        int sideCount = side/((int)(width / GRID_TARGET_COUNT));
        if (sideCount < 2) sideCount = 2;
        
        // layout grid
        createGrid(positions, ColorA(0, 0, 0, 0.2), sideCount, side, boundary.xMid, boundary.yMid);
//        createCircle(...)
        
        // define connections to neighbors
        for(int idx = 0; idx < positions.size(); idx++) {
            int neighbors[4] = {
                idx % sideCount == 0 ? -1 : idx - 1, // start of row?
                idx - sideCount,
                (idx + 1) % sideCount == 0 ? -1 : idx + 1, // end of row?
                idx + sideCount
            };
            vector<int> myConnections;
            for(auto neighbor : neighbors) {
                if (neighbor >= 0 && neighbor < positions.size()) {
                    myConnections.push_back(neighbor);
                }
            }
            connections.push_back(myConnections);
        }
        
        // compute weights and open/closed signs
        for(auto &gp : positions) {
//            // TODO: try some perlin for great justice
//            // TODO: apply color mapping from sampled image

            gp.velocity.x = Rand::randFloat();
            gp.velocity.y = Rand::randFloat() < 0.5 ? -1.0 : 1.0;
        }
    }
    
    void update() override;
    void renderScene(cairo::Context &ctx, int x0, int y0) override;

private:
    vector<Particle> positions;
    vector<vector<int>> connections;
};

void ConnectedPoints::update() {
    int idx = 0;
    for(auto &gp : positions) {
        // calculate average towards neighbors given weights of neighbors
        vec2 move = vec2(0, 0);
        double weight = 0.0;
        vector<int> neighbors = connections[idx];
        for(auto neighbor : neighbors) {
            vec2 nMove = positions[neighbor].position;
            double neighborWeight = positions[neighbor].velocity.x;
            nMove *= neighborWeight;
            
//            cout << neighborWeight << " :: " << nMove << endl;
            
            weight += neighborWeight;
            move += nMove;
        }
//        cout << weight << " ::  " << move << endl;
        move /= weight;
//        cout << weight << " ::  " << move << endl;
//        cout << "my pos: " << gp.position << endl;
        
        // affect posX, posY or position
        // some things happen here...
        vec2 delta = move - gp.position;
        cout << "delta: " << delta << endl;
        // ???
        double dd = sqrt(delta.x*delta.x + delta.y*delta.y || 1.0);
        delta /= dd;
        cout << "scaled?: " << delta << endl;
        
        double a = Rand::randFloat()*M_PI*2.0;
        double r = Rand::randFloat()*NOISE;
        
        delta *= gp.velocity.y; // sign
        delta *= SCALE;
        delta += vec2(cos(a)*r, sin(a)*r);
        
        gp.position += delta;
        
        idx++;
    }
}

void ConnectedPoints::renderScene(cairo::Context &ctx, int x0, int y0) {
    ctx.setSource(foreground);
    int idx = 0;
    for(auto gp : positions) {
        // draw lines to all my neighbors
        vector<int> neighbors = connections[idx];
        for(auto neighbor : neighbors) {
            // connect with neighbor
            sand.stroke(4, gp, positions[neighbor]);
        }
        idx++;
    }
    
    sand.drawFrame(ctx, Rectf(x0, y0, width, height));
}

#endif
