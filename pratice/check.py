#!/usr/bin/env python
from __future__ import print_function
from PIL import Image, ImageDraw


def read_commands(name):
    with open('input/{}.in.out'.format(name)) as f:
        get = lambda func: func(f.readline())
        split = lambda s: s.strip().split(' ')
        n = get(int)
        painted = {}
        for i in xrange(n):
            cmd = get(split)
            if cmd[0] == 'PAINT_LINE':
                r1, c1, r2, c2 = map(int, cmd[1:])
                if c1 > c2:
                    c1, c2 = c2, c1
                if r1 > r2:
                    r1, r2 = r2, r1
                if r1 == r2:
                    for c in xrange(c1, c2 + 1):
                        painted[(r1, c)] = True
                else:
                    for r in xrange(r1, r2 + 1):
                        painted[(r1, c1)] = True
    return painted


def plot_image(name):
    with open('input/{}.in'.format(name)) as f:
        numbers = lambda s: map(int, s.split(' '))
        get = lambda func: func(f.readline())
        strip = lambda s: s.strip()
        (rows, cols) = get(numbers)
        img = Image.new('RGB', (cols, rows))
        black = (0,0,0)
        white = (255,255,255)
        for y in xrange(rows):
            line = get(strip)
            for x in xrange(cols):
                img.putpixel((x, y), white if line[x] == '.' else black)
    return img


def repaint_image(name):
    img1 = plot_image(name)
    painted = read_commands(name)
    img2 = Image.new('RGB', img1.size)
    draw = ImageDraw.Draw(img2)
    draw.rectangle([(0,0), img2.size], 'white')
    for (y,x), color in painted.iteritems():
        draw.point((x, y), 'green' if color else 'white')
    img2.show()


if __name__ == '__main__':
    repaint_image('learn_and_teach')
