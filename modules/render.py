import cairocffi as cairo

from random import randint
from numpy.random import random
from numpy import pi
from numpy import sqrt
from numpy import linspace
from numpy import arctan2
from numpy import cos
from numpy import sin
from numpy import column_stack
from numpy import square
from numpy import array


class Render(object):
    def __init__(self, n, back, front):
        self.n = n
        self.back = back

        self.pixelSize = 1.0/float(n)

        self.__init_cairo()
        self.set_front(front)

    def __init_cairo(self):
        self.surface = cairo.ImageSurface(cairo.FORMAT_ARGB32, int(self.n), int(self.n))
        self.ctx = cairo.Context(self.surface)

        # special drawing surface (0-1) with 0.5,0.5 centered
        self.ctx.scale(self.n, self.n)
        self.ctx.translate(0.5, 0.5)

        self.clear_canvas()

    def clear_canvas(self):
        self.ctx.set_source_rgba(*self.back)
        self.ctx.paint()

    def write_to_png(self, fn):
        self.surface.write_to_png(fn)

    def set_front_from_colors_randomly(self, a=1):
        # pick random index [0-ncolors)
        idx = randint(0, self.ncolors-1)
        self.set_front_from_colors(idx, a)

    def set_front_from_colors(self, i, a=1):
        ii = i%self.ncolors

        r,g,b = self.colors[ii]

        # print(str(r) + "," + str(g) + "," + str(b))

        c = [r,g,b,a]

        self.front = c
        self.ctx.set_source_rgba(*c)

    def get_colors_from_file(self, fn):
        from PIL import Image
        from numpy.random import shuffle

        def p(f):
            return float('{:0.5f}'.format(f))

        scale = 1./255.
        im = Image.open(fn)
        w, h = im.size
        rgbim = im.convert('RGB')
        res = []
        for i in range(0, w):
          for j in range(0, h):
            r, g, b = rgbim.getpixel((i, j))
            res.append([p(r*scale), p(g*scale), p(b*scale)])

        shuffle(res)

        self.colors = res
        self.ncolors = len(res)

    def set_front(self, c):
        self.front = c
        self.ctx.set_source_rgba(*c)

    def set_back(self, c):
        self.back = c

    def scale(self, n):
        self.ctx.scale(n, n)

    def paint_dots(self, xys):
        for xy in xys:
            self.dot(xy[0], xy[1])

    def dot(self, x, y):
        self.ctx.rectangle(x, y, self.pixelSize, self.pixelSize)
        self.ctx.fill()

    def sandstroke(self, xys, grains=10):
        pixelSize = self.pixelSize
        rectangle = self.ctx.rectangle
        fill = self.ctx.fill

        dx = xys[:,2] - xys[:,0]
        dy = xys[:,3] - xys[:,1]

        aa = arctan2(dy,dx)
        directions = column_stack([cos(aa),sin(aa)])

        dd = sqrt(square(dx)+square(dy))

        for i,d in enumerate(dd):
          for x,y in xys[i,:2] + directions[i,:]*random((grains,1))*d:
            rectangle(x,y,pixelSize,pixelSize)
            fill()
