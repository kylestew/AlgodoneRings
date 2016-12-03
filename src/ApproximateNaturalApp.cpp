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
    
    void nextSceneSet();
    
    ColorA background;
    
    vector<Scene*> scenes;
};

void ApproximateNaturalApp::setup() {
    setWindowSize(1200, 900);
    setFullScreen(true);
    
    background = ColorA(0.99,0.99,.94,1);
    
    nextSceneSet();
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
    else if( event.getChar() == 's' ) {
//        cairo::Context ctx( cairo::SurfaceSvg( getHomeDirectory() / "CairoBasicShot.svg", getWindowWidth(), getWindowHeight() ) );
//        renderScene( ctx );
    }
}

void ApproximateNaturalApp::nextSceneSet() {
    // TODO: clear out existing scene set
    
    int width = getWindowWidth();
    int height = getWindowHeight();
    
    scenes.push_back(new Scene(width, height));
    
    
    // TODO: pick variable scene and grid layout
    // TODO: pick random asset path per run if needs color asset
    
    // WALKERS
//    scenes.push_back(new Walkers(WIDTH, HEIGHT, "gradient.png", ColorA(0,0,0,1)));
    
}

void ApproximateNaturalApp::update() {
    for(auto &scene : scenes) {
        scene->update();
    }
}

void ApproximateNaturalApp::draw() {
    cairo::SurfaceQuartz surface = cairo::createWindowSurface();
    int width = surface.getWidth();
    int height = surface.getHeight();
    cairo::Context ctx(surface);
    
    // background color
    ctx.setSource(background);
    ctx.paint();
    
    // draw border
    int padding = 30;
    ctx.setSourceRgb(0.2, 0.2, 0.2);
    ctx.rectangle(padding, padding, width-(padding*2), height-(padding*2));
    ctx.setLineWidth(3);
    ctx.stroke();
    
    padding += 4;
    ctx.rectangle(padding, padding, width-(padding*2), height-(padding*2));
    ctx.setLineWidth(1);
    ctx.stroke();
    
    // scene title
    ctx.setFontFace(cairo::FontFace("MuseoSans"));
    ctx.setFontSize(13);
    
    ctx.moveTo(100, 100);
    ctx.setSourceRgb(0.2, 0.2, 0.2);
    ctx.showText("Hello World");
//    cairo_show_text(cr, "Most relationships seem so transitory");
//    cairo_move_to(cr, 20, 60);
//    cairo_show_text(cr, "They're all good but not the permanent one");
    
//    cairo_move_to(cr, 20, 120);
//    cairo_show_text(cr, "Who doesn't long for someone to hold");
//    
//    cairo_move_to(cr, 20, 150);
//    cairo_show_text(cr, "Who knows how to love you without being told");
//    cairo_move_to(cr, 20, 180);
//    cairo_show_text(cr, "Somebody tell me why I'm on my own");
//    cairo_move_to(cr, 20, 210);
//    cairo_show_text(cr, "If there's a soulmate for everyone");
    
    // now draw grid of scenes
//    for(auto &scene : scenes) {
//        scene->renderScene(ctx, 0, 0);
//    }
}

CINDER_APP( ApproximateNaturalApp, Renderer2d )
