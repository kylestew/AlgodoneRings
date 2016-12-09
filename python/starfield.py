#!/usr/bin/env python
import random
import math
import pygame
from pygame.locals import *

WINSIZE = [640, 480]
WINCENTER = [320, 240]
NUMSTARS = 150

def init_star():
    dir = random.randrange(100000)
    velmult = random.random()*0.6+0.4
    vel = [math.sin(dir) * velmult, math.cos(dir) * velmult]
    return vel, WINCENTER[:]

def initialize_stars():
    "creates a new starfield"
    stars = []
    for x in range(NUMSTARS):
        star = init_star()
        vel, pos = star
        steps = random.randint(0, WINCENTER[0])
        # distribute stars around horizontally?
        # I guess vel, pos point to values in star and let you change them
        pos[0] = pos[0] + (vel[0] * steps)
        pos[1] = pos[1] + (vel[1] * steps)
        vel[0] = vel[0] * (steps * 0.09)
        vel[1] = vel[1] * (steps * 0.09)
        stars.append(star)
    move_stars(stars)
    return stars

def draw_stars(surface, stars, color):
    "used to draw (and clear) the stars"
    for vel, pos in stars:
        pos = (int(pos[0]), int(pos[1]))
        surface.set_at(pos, color)

def move_stars(stars):
    "animate the star values"
    for vel, pos in stars:
        pos[0] = pos[0] + vel[0]
        pos[1] = pos[1] + vel[1]
        if not 0 <= pos[0] <= WINSIZE[0] or not 0 <= pos[1] <= WINSIZE[1]:
            vel[:], pos[:] = init_star()
        else:
            vel[0] = vel[0] * 1.05
            vel[1] = vel[1] * 1.05

def main():
    # create our starfield
    random.seed()
    stars = initialize_stars()
    clock = pygame.time.Clock()
    # initialize and prepare screen
    pygame.init()
    screen = pygame.display.set_mode(WINSIZE);
    pygame.display.set_caption('pygame Stars Example')
    white = 255, 240, 200
    black = 20, 20, 40
    screen.fill(black)

    # main game loop
    done = 0
    while not done:
        draw_stars(screen, stars, black)
        move_stars(stars)
        draw_stars(screen, stars, white)
        pygame.display.update()
        for e in pygame.event.get():
            # if e.type == QUIT or (e.type == KEYDOWN and e.key == K_ESCAPE):
            #     print("ADSF")
            #     done = 1
            #     break
            if e.type == KEYDOWN:
                print("key: " + e.key)
            elif e.type == MOUSEBUTTONDOWN and e.button == 1:
                WINCENTER[:] = list(e.pos)
        clock.tick(50)
    pygame.quit()


if __name__ == '__main__':
    main()
