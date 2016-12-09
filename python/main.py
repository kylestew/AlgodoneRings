#!/usr/bin/env python3
import pygame
from pygame.locals import *
import cairocffi as cairo
from PIL import Image
from random import randint
import math

# scenes
# from hyphae import Hyphae
# from differentialLine import DifferentialLineScene
from sandSplineScene import SandSplineScene


DEV_MODE = True

SCREEN_SIZE = [640, 480]
FPS = 8

MAX_COLUMNS = 6
DISPLAY_SECONDS = 60

FILL_BACK = 255,255,255
BACK = 1,1,1,1
FRONT = 0,0,0,0.01

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
    padding = round(size[0] * 0.02)

    # choose random column count
    cols = randint(1, MAX_COLUMNS)

    # TEMP: TESTING
    if DEV_MODE:
        cols = 1

    # cell size is always square
    nonPadWidth = size[0] - (padding * cols + padding)
    n = round(nonPadWidth/cols)
    # if cell size greater than heigh, we need to bound by height
    if n > size[1]:
        n = size[1] - padding*2 # assuming single cell

    # determine row count that will fill vertically
    rows = math.floor((size[1]-padding)/(n+padding)) # height - 1 slice of padding / padded rows

    # reconfigure the padding so everything is centered
    xPadding = round((size[0] - n*cols)/(cols+1))
    yPadding = round((size[1] - n*rows)/(rows+1))

    # create grid positions (top lefts)
    global sceneGrid
    sceneGrid = []
    xPos = 0
    yPos = yPadding
    for y in range(rows):
        xPos = xPadding
        for x in range(cols):
            sceneGrid.append((xPos, yPos))
            xPos += n + xPadding
        yPos += n + yPadding


    # TODO: better random values for scenes
    # make scenes
    global scenes
    scenes = []
    for i in range(rows*cols):
        scenes.append(SandSplineScene(n, FRONT, BACK))

def main():
    clock = pygame.time.Clock()
    pygame.init()
    screen = pygame.display.set_mode(SCREEN_SIZE);
    # screen = pygame.display.set_mode([0,0], pygame.FULLSCREEN);
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
            # elif e.type == MOUSEBUTTONDOWN and e.button == 1:
            #     WINCENTER[:] = list(e.pos)
            # TODO: dump to PNG command
        clock.tick(FPS) # argument is FPS setting
    pygame.quit()


if __name__ == '__main__':
    main()
