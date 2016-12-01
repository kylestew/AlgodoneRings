#include "cinder/app/App.h"
#include "cinder/cairo/Cairo.h"
#include "cinder/Utilities.h"

#include "Sand.h"
#include "Particle.h"
#include "Utils.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class ApproximateNaturalApp : public App {
  public:
	void setup() override;
    void keyDown(KeyEvent event) override;
	void update() override;
	void renderScene();
	void draw() override;
    
    static const int WIDTH = 900;
    static const int HEIGHT = 900;
    
    constexpr static const double NOISE = 0.05;
    constexpr static const double SCALE = 0.01;
    
    Sand sand; // drawing engine
    
    int sideCount;
    vector<Particle> positions;
};

void ApproximateNaturalApp::setup() {
    setWindowSize(WIDTH, HEIGHT);
    
    sand = Sand(WIDTH, HEIGHT);
    sand.setBackground(ColorA(1,1,1,1));
    //    sand.setGrain(2.0);
    
    // frame in drawing space
    Boundary boundary = getBoundary(WIDTH, HEIGHT, 0.14);

    // decide how many grid rows and columns to place
    int side = min(boundary.innerWidth, boundary.innerHeight); // find smallest side
    sideCount = side/80.0;
    
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



void ApproximateNaturalApp::keyDown( KeyEvent event ) {
//    if( event.getCode() == KeyEvent::KEY_f ) {
//        setFullScreen( ! isFullScreen() );
//    }
//    else if( event.getChar() == 'x' ) {
//        mFlowers.clear();
//    }
//    else if( event.getChar() == 's' ) {
//        cairo::Context ctx( cairo::SurfaceSvg( getHomeDirectory() / "CairoBasicShot.svg", getWindowWidth(), getWindowHeight() ) );
//        renderScene( ctx );
//    }
//    else if( event.getChar() == 'e' ) {
//        cairo::Context ctx( cairo::SurfaceEps( getHomeDirectory() / "CairoBasicShot.eps", getWindowWidth(), getWindowHeight() ) );
//        renderScene( ctx );
//    }
//    else if( event.getChar() == 'p' ) {
//        cairo::Context ctx( cairo::SurfacePs( getHomeDirectory() / "CairoBasicShot.ps", getWindowWidth(), getWindowHeight() ) );
//        renderScene( ctx );
//    }
//    else if( event.getChar() == 'd' ) {
//        cairo::Context ctx( cairo::SurfacePdf( getHomeDirectory() / "CairoBasicShot.pdf", getWindowWidth(), getWindowHeight() ) );
//        renderScene( ctx );
//    }	
}



void ApproximateNaturalApp::update() {
    int idx = 0;
    for(auto &gp : positions) {
        int neighbors[4] = {
            idx % sideCount == 0 ? -1 : idx - 1, // start of row?
            idx - sideCount,
            (idx + 1) % sideCount == 0 ? -1 : idx + 1, // end of row?
            idx + sideCount
        };
        
        // calculate average towards neighbors given weights of neighbors
        vec2 move = vec2(0, 0);
        double weight = 0.0;
        for(auto neighbor : neighbors) {
            if (neighbor >= 0 && neighbor < positions.size()) {
                vec2 nMove = positions[neighbor].position;
                double neighborWeight = positions[neighbor].velocity.x;
                nMove *= neighborWeight;
                
                cout << neighborWeight << " :: " << nMove << endl;
                
                weight += neighborWeight;
                move += nMove;
            }
        }
        cout << weight << " ::  " << move << endl;
        move /= weight;
        cout << weight << " ::  " << move << endl;
        cout << "my pos: " << gp.position << endl;
        
        
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
        
//        gp.position += vec2(dx*SCALE + cos(a)*r, dy*SCALE + sin(a)*r);
        
//        return {
//        x: limit(gx + sgn*dx*SCALE + Math.cos(a)*r,
//                 boundary.xr-1, boundary.xl),
//        y: limit(gy + sgn*dy*SCALE + Math.sin(a)*r,
//                 boundary.yb-1, boundary.yt)
//        };
        
        
        idx++;
    }
}

void ApproximateNaturalApp::renderScene() {
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
                // connect with neighbor
                sand.stroke(4, gp, positions[neighbor]);
            }
        }
        idx++;
    }
}

void ApproximateNaturalApp::draw() {
    cairo::Context ctx(cairo::createWindowSurface());
    renderScene();
    sand.drawFrame(ctx);
}

CINDER_APP( ApproximateNaturalApp, Renderer2d )
