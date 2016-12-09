from modules.render import Render
from modules.sandSpline import SandSpline

class SandSplineScene(Render):
    def __init__(self, n, back, front):
        Render.__init__(self, n, front, back)

        # scale down canvas so randomnes doesn't get clipped
        self.scale(0.6)

        # color or sampling
        # numSplines
        # minSections
        # maxSections
        # noiseScale
        # density
        # ordered T/F

        self.spline = SandSpline(15, 100, 0.00000003, 1000, True)

    def step(self):
        xy = next(self.spline)
        self.paint_dots(xy)
