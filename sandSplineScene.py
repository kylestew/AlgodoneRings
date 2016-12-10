from modules.render import Render
from modules.sandSpline import SandSpline
from random import random, randint



# TODO: SOLOS


class SandSplineParamGenerator():
    def __init__(self):
        density = randint(300, 900)
        alpha = randint(1, 100)/density + 0.01
        self.params = {
            'numSplines': randint(3, 12),
            'sectionCount': randint(15, 100),
            'noiseScale': randint(1, 10)/12000.0,
            'density': density,
            'ordered': True if random() < 0.5 else False,
            'alpha': alpha,
            'radiusOffsetBase': (random()/2.0)+0.5, # 0.5-1.0
            'radiusOffsetDiverge': (random()/2.0), # small random divergence of radius
        }
        self.params['radiusOffsetDiverge'] = 0


    def generate(self):
        return self.params


class SandSplineScene(Render):
    def __init__(self, n, back, front, params):
        Render.__init__(self, n, front, back)

        # scale down canvas so random drift doesn't get clipped
        self.scale(0.75)

        print(params)

        numSplines = params['numSplines']
        sectionCount = params['sectionCount']
        noiseScale = params['noiseScale']
        density = params['density']
        ordered = params['ordered']
        self.alpha = params['alpha']
        radiusOffset = params['radiusOffsetBase']
        radiusOffsetDiverge = params['radiusOffsetDiverge']

        # load up sampler
        self.sampler = self.get_colors_from_file('img/check.png')

        # create splines
        self.splines = []
        for i in range(numSplines):
            radius = radiusOffset + (random()-0.5) * radiusOffsetDiverge
            self.splines.append(SandSpline(sectionCount, radius, noiseScale, density, ordered))

    def step(self):
        for spline in self.splines:
            xy = next(spline)
            self.set_front_from_colors_randomly(a=self.alpha)
            self.paint_dots(xy)
