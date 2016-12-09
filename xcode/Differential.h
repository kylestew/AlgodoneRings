#ifndef Differential_h
#define Differential_h

#include "cinder/Rand.h"

using namespace ci;
using namespace ci::app;

struct DifferentialPoint {
    vec2 pos;
};

class Differential {
public:
    
    Differential() {}
    Differential(vector<DifferentialPoint> points, int maxNum, int near, int far)
    : points(points) {
//        this.near = near;
//        this.far = far;
//        this.far2 = far*far;
//        this.maxNum = maxNum;
//        this.tree = new kdTree(this.points, dst2, ['x', 'y']);
    }
    
    
    static vector<DifferentialPoint> getConnectedCircle(int num, int cx, int cy, double rad) {
        Rand random = Rand();
        random.seed((int)clock());
        
        vector<DifferentialPoint> points;
        for(int i = 0; i < num; ++i) {
            double a = random.nextFloat(0, 2.0*M_PI);
            double x = cx + sin(a)*rad;
            double y = cy + cos(a)*rad;
            points.push_back({
                vec2(x, y)
            });
        }
        
        // add extra shit here?
//            points.forEach((p, i) => {
//                const l = (num + i - 1)%num;
//                const r = (num + i + 1)%num;
//                // TODO: solve this differently?
        // are these points left, right of me?
//                p.l = points[l];
//                p.r = points[r];
//                p.age = INITIALAGE;
//            });
        
        return points;
    }
    
    vector<DifferentialPoint> points;
private:
};

#endif /* Differential_h */
