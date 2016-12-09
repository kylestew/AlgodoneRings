#include "cinder/app/App.h"
#include "cinder/cairo/Cairo.h"
#include "cinder/Utilities.h"

#include "Scene.h"
#include "DifferentialLine.h"
//#include "Walkers.h"
#include "ConnectedPoints.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class ApproximateNaturalApp : public App {
  public:
	void setup() override;
    void keyDown(KeyEvent event) override;
	void update() override;
	void renderScene(cairo::Context &ctx, int width, int height);
	void draw() override;
    
    void nextSceneSet();
    
    static const int BASE_PADDING = 30;
    static const int TOP_PADDING = 90;
    
    ColorA background;
    ColorA foreground;
    
    int rows, cols;
    vec2 cellSize;
    vector<Scene*> scenes;
    
    int currentFrame = 0;
};

void ApproximateNaturalApp::setup() {
    setWindowSize(900, 900);
//    setFullScreen(true);
    
    background = ColorA(0.99, 0.99, 0.94, 1);
    foreground = ColorA(0.22, 0.22, 0.22, 1);
    
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
        cairo::SurfaceCgBitmapContext surface = cairo::SurfaceCgBitmapContext(getWindowWidth(), getWindowHeight(), false);
        cairo::Context ctx(surface);
        renderScene(ctx, surface.getWidth(), surface.getHeight());
        char s[28];
        sprintf(s, "RENDER/dump/out_%08d.png", currentFrame++);
        writeImage(getHomeDirectory() / s, surface.getSurface());
    }
}

void ApproximateNaturalApp::nextSceneSet() {
    // TODO: clear out existing scene set
    
    int width = getWindowWidth();
    int height = getWindowHeight();
    
    // adjust for padding
//    width -= BASE_PADDING*2;
//    height -= BASE_PADDING+TOP_PADDING;
    
//    rows = 2;
//    cols = 3;
//    int num = rows * cols;
//    cellSize = vec2(floor(width/cols), floor(height/rows));
//    for (int i = 0; i < num; ++i) {
//        scenes.push_back(new Scene(cellSize.x, cellSize.y));
//    }
    
    // TODO: pick variable scene and grid layout
    // TODO: pick random asset path per run if needs color asset
    
    // WALKERS
    rows = 1;
    cols = 1;
//    scenes.push_back(new Walkers(width, height, "sampler01.jpg", ColorA(0.1,0.1,0.1,1)));
    
    
    scenes.push_back(new DifferentailLine(width, height, (int)clock(), background, foreground));
}

void ApproximateNaturalApp::update() {
    for(auto &scene : scenes) {
        scene->update();
    }
}

void ApproximateNaturalApp::renderScene(cairo::Context &ctx, int width, int height) {
    // background color
    ctx.setSource(background);
    ctx.paint();
    
    // draw border
    int padding = 0;
//    int padding = BASE_PADDING;
//    ctx.setSource(foreground);
//    ctx.rectangle(padding, padding, width-(padding*2), height-(padding*2));
//    ctx.setLineWidth(3);
//    ctx.stroke();
//    
//    padding += 4;
//    ctx.rectangle(padding, padding, width-(padding*2), height-(padding*2));
//    ctx.setLineWidth(1);
//    ctx.stroke();
//    
//    // scene title
//    ctx.setFontFace(cairo::FontFace("Museo Sans"));
//    ctx.setFontSize(38);
//    // have to manually center text by measuring and offsetting it
//    int x = width/2.0;
//    int y = padding + 18.0;
//    const char* utf8 = scenes[0]->sceneName;
//    cairo::TextExtents extents = ctx.textExtents(utf8);
//    x -= (extents.width()/2 + extents.xBearing());
//    y += extents.height();
//    ctx.moveTo(x, y);
//    ctx.showText(utf8);
    
    // now draw grid of scenes
    int xOff = padding;
    int yOff = TOP_PADDING;
    yOff = 0; // TEMP
    int idx = 0;
    for(auto &scene : scenes) {
        scene->renderScene(ctx, xOff, yOff);
        xOff += cellSize.x;
        if (++idx % cols == 0) {
            xOff = padding;
            yOff += 660 * 2;
        }
        cout << yOff << endl;
    }
}

void ApproximateNaturalApp::draw() {
    cairo::SurfaceQuartz surface = cairo::createWindowSurface();
    cairo::Context ctx(surface);
    renderScene(ctx, surface.getWidth(), surface.getHeight());
}

CINDER_APP( ApproximateNaturalApp, Renderer2d )
