from modules.render import Render

CONTRASTA = [0.84,0.37,0] # orange
CONTRASTB = [0.53,0.53,1] # lightblue
CONTRASTC = [0.84,1.00,0]

class DifferentialLine(Render):
    def __init__(self, n, back, front):
        Render.__init__(self, n, front, back)

        # init data now

    def step(self):
        # step forever - no stop condition
        "step"
