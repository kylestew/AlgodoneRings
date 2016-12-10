from modules.render import Render
from modules.sandSpline import SandSpline
from random import random, randint


class SandSplineParamGenerator():
    def __init__(self, count):

        numSplines = randint(1, 16)
        sectionCount = randint(8, 32)
        noiseScale = randint(20, 30)/2000000.0
        if count > 2:
            noiseScale *= count * 0.75
        noiseDetail = randint(10, 2000)
        density = randint(500, 1500)
        radius = random()/2.0+0.5
        decayRadius = True if random() < 0.6 else False
        radiusDecay = random() * 0.05

        self.params = {
            'numSplines': numSplines,
            'sectionCount': sectionCount,
            'noiseScale': noiseScale,
            'noiseDetail': noiseDetail,
            'density': density,
            'radius': radius,
            'decayRadius': decayRadius,
            'radiusDecay': radiusDecay,
        }

    def generate(self):
        return self.params



class SandSplineScene(Render):
    def __init__(self, n, back, front, params):
        Render.__init__(self, n, front, back)

        # scale down canvas so random drift doesn't get clipped
        self.scale(0.6)

        numSplines = params['numSplines']
        sectionCount = params['sectionCount']
        noiseScale = params['noiseScale']
        noiseDetail = params['noiseDetail']
        density = params['density']
        radius = params['radius']
        decayRadius = params['decayRadius']
        radiusDecay = params['radiusDecay']

        # computed
        self.alpha = n * 0.00006 * (1.0+random()*0.1)
        if self.alpha < 0.05:
            self.alpha = 0.08

        print('===========')
        print("spline count: " + str(numSplines))
        print("section count: " + str(sectionCount))
        print("noise: " + str(noiseScale))
        print("detail: " + str(noiseDetail))
        print("alpha: " + str(self.alpha))
        print("radiusDecay: " + str(radiusDecay))


        # load up sampler
        self.sampler = self.get_colors_from_file('img/check.png')

        # create splines
        self.splines = []
        for i in range(numSplines):
            self.splines.append(SandSpline(sectionCount, radius, noiseScale, noiseDetail, density))
            if decayRadius:
                radius -= radiusDecay

    def step(self):
        for spline in self.splines:
            xy = next(spline)
            self.set_front_from_colors_randomly(a=self.alpha)
            self.paint_dots(xy)
