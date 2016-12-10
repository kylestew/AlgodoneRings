import numpy as np
from numpy import pi
from numpy.random import random
from numpy.random import randint
from scipy.interpolate import splprep
from scipy.interpolate import splev
from noise import snoise2

class SandSpline(object):
    # int, int, float32
    def __init__(self, sectionCount, radiusOffset, noiseScale, noiseDetail, density):
        self.sectionCount = sectionCount
        self.noiseScale = noiseScale
        self.noiseDetail = noiseDetail
        self.density = density

        # select angles around a full circle evenly spaced
        # pick a random starting angle so each spline doesn't start at the same spot
        angleStops = random()*pi*2.0 + np.linspace(0, pi*2.0, sectionCount)

        # translate to points around a unit circle (-0.5,0.5)
        # offset radius so points aren't completly circular
        self.path = np.column_stack((np.cos(angleStops), np.sin(angleStops))) * (0.5*radiusOffset)

        # noise starts at zero - grows with each 'next' call
        self.noise = np.zeros(sectionCount, 'float')

        self.i = randint(0, 1000)

    def __iter__(self):
        return self

    def __next__(self):
        num = len(self.path) # number of segments in spline

        # randomly permute path positions
        # generate a random number from (-1,1) for each spline section
        r = np.zeros(num)
        for i in range(num):
            r[i] = snoise2(i, self.i/self.noiseDetail)
        # dampen random values
        r *= self.noiseScale
        # permute our noise value by applying random values
        # remember they are centered around 0 so noise will not drift
        self.noise[:] += r

        # pick a new random set or angles to drive towards
        a = np.zeros(num)
        for i in range(num):
            a[i] = 2.0*pi*snoise2(i, self.i/self.noiseDetail)
        rnd = np.column_stack((np.cos(a), np.sin(a)))
        # using noise, move current points towards random points a little
        self.path += rnd * np.reshape(self.noise, (num, 1))

        # always force last item to equal first so there isn't disconnection in spline
        self.path[num-1] = self.path[0]

        # output an interpolate spline path from the points
        self.interpolatedPath = self.randomInterpolation(self.path, self.density)

        self.i += 1
        return self.interpolatedPath

    def randomInterpolation(self, path, numPoints):
        # b-spline representation of n-dimensional curve
        tck, u = splprep([
            path[:,0],
            path[:,1]],
            s=0
        )
        unew = random(numPoints)
        unew = np.sort(unew)
        # evaluate the value of the smoothing polynomail for the b-spline
        out = splev(unew, tck)
        return np.column_stack(out)
