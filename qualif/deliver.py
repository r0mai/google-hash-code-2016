#!/usr/bin/env python
from __future__ import print_function
import math
import sys


index = lambda n: lambda x: x[n]
first = index(0)
second = index(1)
split = lambda d: lambda s: s.split(d)
strip = lambda s: s.strip()
numbers = lambda s: map(int, s.split(' '))



def distance(p, q):
    x = (p.row - q.row)**2 + (p.col - q.col)**2
    return int(math.ceil(math.sqrt(x)))


def get_data(fname):
    return get_simulation(fname), get_commands(fname)


def get_simulation(fname):
    with open(fname) as f:
        get = lambda func: func(f.readline())
        R, C, D, T, M = get(numbers)
        P = get(int)
        ps = get(numbers)
        W = get(int)
        ws = []
        for i in xrange(W):
            pos = get(numbers)
            items = get(numbers)
            ws.append(Warehouse(i, pos, items))
        C = get(int)
        cs = []
        for i in xrange(C):
            pos = get(numbers)
            n = get(int)
            ns = get(numbers)
            cs.append(Order(i, pos, ns))
    return Simulation(D, T, M, ps, ws, cs)


def get_commands(fname):
    with open(fname + '.out') as f:
        get = lambda func: func(f.readline())
        n = get(int)
        cs = []
        for i in xrange(n):
            line = get(split(' '))
            di = int(line[0])
            cmd = line[1]
            args = map(int, line[2:])
            cs.append(Command(cmd, di, args))
    return cs



def is_valid(elem):
    for k, v in elem.payload.iteritems():
        if v < 0:
            return False
    return True


class Command:
    def __init__(self, cmd, di, args):
        self.cmd = cmd
        self.args = [di] + args


class Warehouse:
    def __init__(self, index, pos, items):
        self.index = index
        self.row = pos[0]
        self.col = pos[1]
        self.payload = {}
        for i, v in enumerate(items):
            self.payload[i] = v

    def __repr__(self):
        return '<Warehouse[{}]: {}>'.format(self.index, self.payload)


class Order:
    def __init__(self, index, pos, items):
        self.index = index
        self.row = pos[0]
        self.col = pos[1]
        self.payload = {}
        self.turn = None
        for i in items:
            self.payload[i] = self.payload.get(i, 0) + 1

    def check_complete(self, t):
        for k, v in self.payload.iteritems():
            if v > 0:
                return
        self.turn = t

    def __repr__(self):
        return '<Order[{}]: {} {}>'.format(self.index, self.turn, self.payload)


class Drone:
    def __init__(self, index, pos):
        self.index = index
        self.row = pos.row
        self.col = pos.col
        self.payload = {}
        self.turn = 0

    def load(self, p, n):
        self.payload[p] = self.payload.get(p, 0) + n
        self.turn += 1

    def fly(self, pos):
        self.turn += distance(self, pos)
        self.row = pos.row
        self.col = pos.col

    def unload(self, p, n):
        self.payload[p] = self.payload.get(p, 0) - n
        self.payload = dict([(p, v) for p, v in self.payload.items() if v > 0])
        self.turn += 1

    def __repr__(self):
        return '<Drone[{}]: t{} p{},{} {}>'.format(
            self.index, self.turn, self.row, self.col, self.payload)


class Simulation:
    def __init__(self, D, T, M, ps, ws, cs):
        self.T = T
        self.M = M
        self.ps = ps
        self.ws = ws
        self.cs = cs
        self.ds = [Drone(i, ws[0]) for i in xrange(D)]

    def weight(self, elem):
        w = 0
        for i, n in elem.payload.iteritems():
            w += self.ps[i] * n
        return w

    def show_orders(self):
        print(self.M)
        print('--')
        ws = []
        for i in self.cs:
            ws.append(self.weight(i))
        ws.sort()
        print(ws)

    def load_drone(self, di, wi, pi, n):
        drone = self.ds[di]
        ware = self.ws[wi]
        drone.fly(ware)
        drone.load(pi, n)
        ware.payload[pi] = ware.payload.get(pi, 0) - n

        if self.weight(drone) > self.M:
            print('error: too much load')
            sys.exit(1)

        if not is_valid(ware):
            print('error: no such item in warehouse')
            print(drone)
            print(ware)
            sys.exit(1)

    def deliver_items(self, di, ci, pi, n):
        drone = self.ds[di]
        order = self.cs[ci]
        drone.fly(order)
        drone.unload(pi, n)
        if not is_valid(drone):
            print('error: drone did not have enough items')
            sys.exit(1)

        order.payload[pi] = order.payload.get(pi, 0) - n
        if not is_valid(order):
            print('error: customer got more items')
            print(order)
            sys.exit(1)

        order.check_complete(drone.turn - 1)

    def execute_command(self, cmd):
        drone = self.ds[cmd.args[0]]
        if cmd.cmd == 'L':
            self.load_drone(*cmd.args)
        elif cmd.cmd == 'D':
            self.deliver_items(*cmd.args)
        else:
            print('error: invalid command')
            sys.exit(1)

    def score(self):
        result = 0
        for order in self.cs:
            if order.turn is None:
                continue
            if order.turn > self.T:
                continue
            d, r = divmod((self.T - order.turn) * 100, self.T)
            if r > 0:
                d += 1
            result += d
        return result


if __name__ == '__main__':
    mother = 'inputs/mother_of_all_warehouses.in'
    busy_day = 'inputs/busy_day.in'
    redundancy = 'inputs/redundancy.in'
    example = 'inputs/example.in'

    sim, cmds = get_data(mother)
    for i in cmds:
        sim.execute_command(i)

    # print('--')
    # for x in sim.ds:
    #     print(x)
    # print('--')
    # for x in sim.cs:
    #     print(x)

    print(sim.score())


