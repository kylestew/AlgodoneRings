import cairocffi as cairo
from numpy.random import random
from numpy import cos, sin, pi, arctan2, sqrt,\
                  square, int, linspace, any, all, floor, round, ceil
import numpy as np
from collections import deque


NMAX = 2*1e7 # max number of nodes
# ZONES = N/20 # number of zones on each axis

CONTRASTA = [0.84,0.37,0] # orange
CONTRASTB = [0.53,0.53,1] # lightblue
CONTRASTC = [0.84,1.00,0]

RAD_SCALE = 0.9

SOURCE_NUM = 5
CIRCLE_RADIUS = 0.45


class Hyphae(object):
    def __init__(self, n):
        self.n = n

        self.pixelSize = 1.0/n
        self.dotRadius = 10*self.pixelSize

        self.__init_cairo()
        self.__init_data()

    def __init_cairo(self):
        self.surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, self.n, self.n)
        self.ctx = cairo.Context(self.surface)
        self.ctx.scale(self.n, self.n)
        self.ctx.set_source_rgb(*CONTRASTB)
        self.ctx.paint()
        self.ctx.set_line_width(self.pixelSize)

    def __init_data(self):

        self.X = np.zeros(NMAX,'float') # x position
        self.Y = np.zeros(NMAX,'float') # y position

        self.DQ = deque()

        self.num = 0

        rot = random()*pi*2

        # seed each source branch?
        # for i in range(SOURCE_NUM):
        #     # randomly on canvas
        #     # TODO: optional
        #
        #     # on circle
        #     x = 0.5 + sin(rot + (i*pi*2)/float(SOURCE_NUM))*0.1
        #     y = 0.5 + cos(rot + (i*pi*2)/float(SOURCE_NUM))*0.1
        #
        #     self.X[i] = x
        #     self.Y[i] = y
        #     # ...
        #     self.DQ.append(i)
        #
        #     # draw indicator circle
        #     self.ctx.set_source_rgba(*CONTRASTA)
        #     self.circle(x, y, self.dotRadius*0.5)

    def circle(self, x, y, r):
        self.ctx.arc(x,y,r,0,pi*2.)
        self.ctx.fill()

    def step(self):
        num = self.num
