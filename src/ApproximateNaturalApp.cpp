#include "cinder/app/App.h"
#include "cinder/cairo/Cairo.h"
#include "cinder/Utilities.h"

#include "Scene.h"
#include "RandomGrids.h"
#include "Walkers.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class ApproximateNaturalApp : public App {
  public:
	void setup() override;
    void keyDown(KeyEvent event) override;
	void update() override;
	void draw() override;
    
    ColorA background;
    
    vector<Scene*> scenes;
};

void ApproximateNaturalApp::setup() {
    setWindowSize(1200, 900);
    setFullScreen(true);
    
    int width = getWindowWidth();
    int height = getWindowHeight();
    cout << width << ", " << height << endl;
    
    background = ColorA(0.99,0.99,.94,1);
    
    // TODO: pick variable scene and grid layout
    // TODO: pick random asset path per run if needs color asset
    
    // WALKERS
//    scenes.push_back(new Walkers(WIDTH, HEIGHT, "gradient.png", ColorA(0,0,0,1)));
}

void ApproximateNaturalApp::keyDown( KeyEvent event ) {
    if( event.getCode() == KeyEvent::KEY_RETURN ) {
        setFullScreen( !isFullScreen() );
    }
    else if( event.getCode() == KeyEvent::KEY_ESCAPE ) {
        quit();
    }
    else if( event.getCode() == KeyEvent::KEY_SPACE ) {
        assert(false); // TODO: skip to next design
    }
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
//    for(auto &scene : scenes) {
//        scene->update();
//    }
}

void ApproximateNaturalApp::draw() {
    cairo::SurfaceQuartz surface = cairo::createWindowSurface();
    int width = surface.getWidth();
    int height = surface.getHeight();
    cairo::Context ctx(surface);
    
    // background color
    ctx.setSource(background);
    ctx.paint();
    
    // TODO: draw border
    int padding = 48;
    ctx.setSourceRgb(0, 0, 0);
    ctx.rectangle(padding, padding, width-(padding*2), height-(padding*2));
    ctx.setLineWidth(4);
    ctx.stroke();
    // TODO: scene title
    
    
//    for(auto &scene : scenes) {
//        scene->renderScene(ctx, 0, 0);
//    }
}

CINDER_APP( ApproximateNaturalApp, Renderer2d )
