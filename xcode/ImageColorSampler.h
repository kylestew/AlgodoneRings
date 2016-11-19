#ifndef ImageColorSampler_h
#define ImageColorSampler_h

using namespace ci;
using namespace ci::app;

class ImageColorSampler {
public:
    ImageColorSampler(const fs::path &assetPath) {
        surface = loadImage(loadAsset(assetPath));
        w = surface.getWidth();
        h = surface.getHeight();
    }
    
    ColorA randomSample() {
        double x = Rand::randFloat(0, w);
        double y = Rand::randFloat(0, h);
        return surface.getPixel(vec2(x, y));
    }
    
private:
    Surface32f surface;
    float w, h;
};

#endif /* ImageColorSampler_h */
