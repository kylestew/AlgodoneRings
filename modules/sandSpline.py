import numpy as np
from numpy import pi
from numpy.random import random
from numpy.random import randint
from scipy.interpolate import splprep
from scipy.interpolate import splev
from noise import snoise2

class SandSpline(object):
    # int, int, float32
    def __init__(self, sectionCount, radiusOffset, noiseScale, density, ordered):
        self.sectionCount = sectionCount
        self.noiseScale = noiseScale
        self.density = density
        self.ordered = ordered

        # select angles around a full circle evenly spaced
        # pick a random starting angle so each spline doesn't start at the same spot
        angleStops = random()*pi*2.0 + np.linspace(0, pi*2.0, sectionCount)
        # translate to points around a unit circle (-0.5,0.5)
        # offset radius so points aren't completly circular
        self.path = np.column_stack((np.cos(angleStops), np.sin(angleStops))) * (0.5*radiusOffset)

        # noise starts at zero - grows with each 'next' call
        self.noise = np.zeros(sectionCount, 'float')

        self.i = 0

    def __iter__(self):
        return self

    def __next__(self):
        # randomly permute path positions
        num = len(self.path) # number of segments in spline
        # generate a random number from (-1,1) for each spline section

        # standard random
        # r = 1.0-2.0*random(num)

        # perlin
        r = []
        for itm in np.linspace(0.0, 1.0, num):
            r.append(1.0-2.0*snoise2(itm, self.i/100.0)) # TODO: 100 can be a variable
        r = np.array(r)


        # dampen random values
        r *= self.noiseScale
        # permute our noise value by applying random values
        # remember they are centered around 0
        self.noise[:] += r

        # pick a new random set or angles to drive towards
        # a = random(num)*pi*2.0
        a = []
        for itm in np.linspace(0.0, 1.0, num):
            a.append(2.0*pi*snoise2(itm, self.i/100.0)) # TODO: 100 can be a variable
        a = np.array(a)

        rnd = np.column_stack((np.cos(a), np.sin(a)))
        # using noise, move current points towards random points a little
        self.path += rnd * np.reshape(self.noise, (num, 1))

        # output an interpolate spline path from the points
        self.interpolatedPath = self.randomInterpolation(self.path, self.density, self.ordered)

        self.i += 1
        return self.interpolatedPath

    def randomInterpolation(self, path, numPoints, ordered=False):
        # b-spline representation of n-dimensional curve
        tck, u = splprep([
            path[:,0],
            path[:,1]],
            s=0
        )
        unew = random(numPoints)
        if ordered:
            unew = np.sort(unew)
        # evaluate the value of the smoothing polynomail for the b-spline
        out = splev(unew, tck)
        return np.column_stack(out)
