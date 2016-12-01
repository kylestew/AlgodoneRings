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
    
    Sand sand; // drawing engine
    
    int sideCount;
    vector<Particle> positions;
};

void ApproximateNaturalApp::setup() {
    setWindowSize(WIDTH, HEIGHT);
    
    sand = Sand(WIDTH, HEIGHT);
    sand.setBackground(ColorA(1,1,1,1));
//    sand.setAlpha(1.0);
    //    sand.setGrain(2.0);
    
    // frame in drawing space
    Boundary boundary = getBoundary(WIDTH, HEIGHT, 0.14);

    // decide how many grid rows and columns to place
    int side = min(boundary.innerWidth, boundary.innerHeight); // find smallest side
    sideCount = side/80.0;
    //    int totalCount = sideCount*sideCount;
    
    // layout grid
    createGrid(positions, ColorA(0, 0, 0, 1), sideCount, side, boundary.xMid, boundary.yMid);
    
    // pre-compute random weights and directions
    //  const weight = getRnd(num, 0, 1.0);
    //  const sign = getPMRdn(num);
    //  const noise = 0.3;
    //  const scale = 0.1;
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
    cout << "update" << endl;
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
