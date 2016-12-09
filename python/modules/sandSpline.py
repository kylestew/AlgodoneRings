import numpy as np
from numpy import pi
from numpy.random import random
from numpy.random import randint
from scipy.interpolate import splprep
from scipy.interpolate import splev


class SandSpline(object):
    # int, int, float32
    def __init__(self, minSections, maxSections, noiseScale, density, ordered):
        self.noiseScale = noiseScale
        self.density = density
        self.ordered = ordered

        # pick a random number of sections
        sectionCount = randint(minSections, maxSections)
        # select angles around a full circle evenly spaced
        # pick a random starting angle so each spline doesn't start at the same spot
        angleStops = random()*pi*2.0 + np.linspace(0, pi*2.0, sectionCount)
        # translate to points around a unit circle (-0.5,0.5)
        # offset radius so points aren't completly circular
        self.path = np.column_stack((np.cos(angleStops), np.sin(angleStops))) * (0.1+random()*0.4)

        # noise starts at zero - grows with each 'next' call
        self.noise = np.zeros(sectionCount, 'float')

        self.i = 0

    def __iter__(self):
        return self

    def __next__(self):
        # randomly permute path positions
        num = len(self.path) # number of segments in spline
        # generate a random number from (-1,1) for each spline section
        r = 1.0-2.0*random(num)
        # dampen random values
        r *= self.noiseScale
        # permute our noise value by applying random values
        # remember they are centered around 0
        self.noise[:] += r


        a = random(num)*pi*2.0
        rnd = np.column_stack((np.cos(a), np.sin(a)))
        self.path += rnd * np.reshape(self.noise, (num, 1))

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
