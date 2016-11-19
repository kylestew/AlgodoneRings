#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

#include "Sand.h"
#include "ImageColorSampler.h"
//#include "Walkers.h"

using namespace ci;
using namespace ci::app;
using namespace std;


class ApproximateNaturalApp : public App {
  public:
	void setup() override;
	void update() override;
	void draw() override;
    
    static const int WIDTH = 1200;
    static const int HEIGHT = 900;
//    static const int WALKER_IN_LINE = 500;
    static constexpr float RANDOM_SCALE = 0.001;
    
    Sand sand;
    
//    vector<Walkers> lines;
    vector<Particle> particles;
};

void ApproximateNaturalApp::setup() {
    setWindowSize(WIDTH, HEIGHT);
    setFrameRate(10);
    
    sand = Sand(WIDTH, HEIGHT);
    sand.setBackground(ColorA(1, 1, 1, 1));
    sand.setAlpha(0.02);
    sand.setGrain(0.1);
    
    // sample colors from image
    ImageColorSampler sampler("sample.jpg");
    
    // setup particle grid
    for (int col = 1; col < 11; ++col) {
        for (int row = 1; row < 11; ++row) {
            double x = lmap((double)col, 0.0, 11.0, 0.0, 1.0);
            double y = lmap((double)row, 0.0, 11.0, 0.0, 1.0);
            
//            particles.push_back(Particle(vec2(x, y), sampler.randomSample()));
            particles.push_back(Particle(vec2(x, y), ColorA(0, 0, 0, 1)));
        }
    }
    
    
    // TODO: walker line
    
    
    // define a line
//    particles.push_back(Particle(vec2(0.25, 0.25), sampler.randomSample()));
//    particles.push_back(Particle(vec2(0.75, 0.75), sampler.randomSample()));
}

void ApproximateNaturalApp::update() {
//    sand.drawParticles(particles);
    sand.connectParticles(particles);
}

void ApproximateNaturalApp::draw() {
//    // display current state of surface
	gl::clear(Color(0, 0, 0));
    sand.display();
}

CINDER_APP( ApproximateNaturalApp, RendererGl(RendererGl::Options().msaa(16)) )
