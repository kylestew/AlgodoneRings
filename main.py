#!/usr/bin/env python3
import pygame
from pygame.locals import *
import cairocffi as cairo
from PIL import Image
from random import randint
import math
from datetime import datetime

# scenes
from sandSplineScene import SandSplineScene
from sandSplineScene import SandSplineParamGenerator


DEV_MODE = True

SCREEN_SIZE = [1600, 1200]
FPS = 30

DISPLAY_SECONDS = 3*60

FILL_BACK = 28,77,95 # from inconvergent - love the color scheme
BACK = 28/255,77/255,95/255,1
FRONT = 1,1,1,1

sceneGrid = []
scenes = []


def bgra_surf_to_rgba_string(cairo_surface):
    # We use PIL to do this
    img = Image.frombuffer(
        'RGBA', (cairo_surface.get_width(),
                 cairo_surface.get_height()),
        cairo_surface.get_data(), 'raw', 'BGRA', 0, 1)
    return img.tobytes('raw', 'RGBA', 0, 1)

def prepareScene(size):
    # padding is 4% of width
    padding = round(size[0] * 0.04)

    # choose random column count
    cols = randint(1, 3)
    rows = randint(1, 2)
    if cols == 1: # don't stack on widescreen
        rows = 1

    # TEMP: TESTING
    # if DEV_MODE:
    #     cols = 1
    #     rows = 1

    # determine horizontally constrained cell size
    nonPadWidth = size[0] - (padding * cols + padding)
    nW = round(nonPadWidth/cols)
    # determine vertically constrained cell size
    nonPadHeight = size[1] - (padding * rows + padding)
    nH = round(nonPadHeight/rows)
    # choose the smallest cell size
    n = nW if nW < nH else nH

    # determine gridwork insets that will center the grid
    xInset = (size[0] - ((n+padding)*cols - padding)) / 2.0
    yInset = (size[1] - ((n+padding)*rows - padding)) / 2.0

    # create grid positions (top lefts)
    global sceneGrid
    sceneGrid = []
    xPos = 0
    yPos = yInset
    for y in range(rows):
        xPos = xInset
        for x in range(cols):
            sceneGrid.append((xPos, yPos))
            xPos += n + padding
        yPos += (n + padding)


    # use a parameter generator to created isolated random permutations of parameters
    # only one value will differ randomly
    paramGenerator = SandSplineParamGenerator()

    global scenes
    scenes = []
    for i in range(rows*cols):
        scenes.append(SandSplineScene(n, FRONT, BACK, paramGenerator.generate()))

def saveScreen(screen):
    tf = '%Y%m%d{:s}%H%M%S{:s}%f'.format('-', '-')
    t = datetime.now().strftime(tf)
    pygame.image.save(screen, "captures/screenshot_" + str(t) + ".tif")

def main():
    clock = pygame.time.Clock()
    pygame.init()
    if DEV_MODE:
        screen = pygame.display.set_mode(SCREEN_SIZE)
    else:
        screen = pygame.display.set_mode([0,0], pygame.FULLSCREEN)
    lastSceneStartTime = -10000000 # cause the scene to rebuild on first frame

    done = 0
    while not done:
        # rebuild the scene every so often
        if (pygame.time.get_ticks() - lastSceneStartTime) / 1000 > DISPLAY_SECONDS:
            prepareScene(screen.get_size())
            lastSceneStartTime = pygame.time.get_ticks()

        screen.fill(FILL_BACK)

        # step all scenes
        for scene in scenes:
            scene.step()

        # blit cairo surfaces to screen in grid layout
        for i, xy in enumerate(sceneGrid):
            scene = scenes[i]
            data_string = bgra_surf_to_rgba_string(scene.surface)
            pygame_surface = pygame.image.frombuffer(data_string, (scene.n, scene.n), 'RGBA')
            screen.blit(pygame_surface, xy)

        pygame.display.update()

        for e in pygame.event.get():
            if e.type == QUIT or (e.type == KEYDOWN and e.key == K_ESCAPE):
                done = 1
                break

            # enter - reset scene
            elif e.type == KEYDOWN and e.key == K_RETURN:
                saveScreen(screen) # capture current
                lastSceneStartTime = -10000000

            # space to screen cap
            elif e.type == KEYDOWN and e.key == K_SPACE:
                saveScreen(screen)

        clock.tick(FPS) # argument is FPS setting
        # print("fps:", clock.get_fps())

    pygame.quit()


if __name__ == '__main__':
    main()
