#!/usr/bin/env python
from __future__ import print_function


def save_image(name):
    from PIL import Image
    with open('input/{}.in'.format(name)) as f:
        numbers = lambda s: map(int, s.split(' '))
        get = lambda func: func(f.readline())
        strip = lambda s: s.strip()
        (rows, cols) = get(numbers)
        img = Image.new('1', (cols, rows))
        for y in xrange(rows):
            line = get(strip)
            for x in xrange(cols):
                img.putpixel((x, y), 0 if line[x] == '.' else 1)
        img.save('input/{}.png'.format(name))


def dump_json(name):
    import json
    with open('input/{}.in'.format(name)) as f:
        numbers = lambda s: map(int, s.split(' '))
        get = lambda func: func(f.readline())
        strip = lambda s: s.strip()
        (rows, cols) = get(numbers)
        data = []
        for y in xrange(rows):
            line = get(strip)
            for x in xrange(cols):
                if line[x] == '#':
                    data.append(dict(x=x, y=y))
        with open('input/{}.js'.format(name), 'wb') as g:
            g.write('var original =\n')
            g.write(json.dumps(data, indent=2))
            g.write(';\n')


if __name__ == '__main__':
    dump_json('logo')
    dump_json('learn_and_teach')
    dump_json('right_angle')
    # save_image('logo')
    # save_image('right_angle')
    # save_image('right_angle')
