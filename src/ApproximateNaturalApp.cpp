#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "Utils.h"
#include "Sand.h"
//#include "Walkers.h"

using namespace ci;
using namespace ci::app;
using namespace std;

static const int WIDTH = 900;
static const int HEIGHT = 900;

class ApproximateNaturalApp : public App {
  public:
	void setup() override;
	void update() override;
	void draw() override;
    
    static const int WALKERS_COUNT = WIDTH*2;
    static constexpr float NOISE_SCALE = 0.05;
    static constexpr float NOISE_DETAIL = 0.005;
    static constexpr float NOISE_SPEED = 0.2;
    
    Sand sand; // drawing engine
    
    int sideCount;
    vector<Particle> positions;
};

void prepareSettings(ApproximateNaturalApp::Settings *settings) {
//    settings->setHighDensityDisplayEnabled(); // try removing this line
}

void ApproximateNaturalApp::setup() {
    setWindowSize(WIDTH/getWindowContentScale(), HEIGHT/getWindowContentScale());
    
    sand = Sand(WIDTH, HEIGHT);
    sand.setBackground(ColorA(0, 0, 0, 1));
    sand.setAlpha(0.05);
    sand.setAlpha(1);
//    sand.setGrain(2.0);
    
    // frame in drawing space
    Boundary boundary = getBoundary(WIDTH, HEIGHT, 0.14);
    
    // decide how many grid rows and columns to place
    int side = min(boundary.innerWidth, boundary.innerHeight); // find smallest side
    sideCount = side/80.0;
//    int totalCount = sideCount*sideCount;
    
    // layout grid
    createGrid(positions, ColorA(1, 1, 1, 1), sideCount, side, boundary.xMid, boundary.yMid);
    
    // pre-compute random weights and directions
    //  const weight = getRnd(num, 0, 1.0);
    //  const sign = getPMRdn(num);
    //  const noise = 0.3;
    //  const scale = 0.1;
}

void ApproximateNaturalApp::update() {
    int idx = 0;
    for(auto gp : positions) {
        
        
        // draw lines to all my neighbors
        int neighbors[4] = {
            idx % sideCount == 0 ? -1 : idx - 1, // start of row?
            idx - sideCount,
            (idx + 1) % sideCount == 0 ? -1 : idx + 1, // end of row?
            idx + sideCount
        };
        for(auto neighbor : neighbors) {
            if (neighbor >= 0 && neighbor < positions.size()) {
                
                cout << idx << " <--> " << neighbor << endl;
                
                // connect
                sand.stroke(4, gp, positions[neighbor]);
                
                // ???
//                const nwij = weight[nij];
//                wt += nwij;
//                mx += nwij * gn.x;
//                my += nwij * gn.y;
            }
        }
        
        
        
        idx++;
    }
    
    
    
    
    
            /*
        positions = positions.map((gp, ij) => {
             // get original grid position
             const g = grid[ij];
             // our x, y mapped into space
             const gx = gp.x;
             const gy = gp.y;
             // sign for position
             const sgn = sign[ij];
             
             //???
             let mx = 0;
             let my = 0;
             let wt = 0;
             
             // draw lines to neighbors?
             g.near.forEach((nij) => {
               const gn = positions[nij];
               stroke(ctx, 4, gp, gn);
             
               const nwij = weight[nij];
               wt += nwij;
               mx += nwij * gn.x;
               my += nwij * gn.y;
             });
             
             mx /= wt;
             my /= wt;
             
             let dx = mx - gx;
             let dy = my - gy;
             const dd = Math.sqrt(dx*dx + dy*dy || 1.0);
             dx /= dd;
             dy /= dd;
             
             const a = Math.random()*TWOPI;
             const r = Math.random()*noise;
             
             return {
             x: limit(gx + sgn*dx*scale + Math.cos(a)*r,
             boundary.xr-1, boundary.xl),
             y: limit(gy + sgn*dy*scale + Math.sin(a)*r,
             boundary.yb-1, boundary.yt)
             };
        });
             */
}

void ApproximateNaturalApp::draw() {
    // display current state of surface
	gl::clear(Color(0, 0, 0));
//    sand.display(2);
    sand.display();
}

CINDER_APP( ApproximateNaturalApp, RendererGl(RendererGl::Options().msaa(16)), prepareSettings )
