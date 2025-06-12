# import re
import numpy as np
from numpy.typing import NDArray

input = open("../input.txt", "r").read().splitlines()
# print(input)

size = 1000
grid = np.zeros((size, size), dtype=int)


def turn(operation: str, grid: NDArray, region):
    x0, y0 = region[0]
    x1, y1 = region[1]
    match operation:
        case "on":
            val = 1
        case "off":
            val = 0
        case _:
            raise NotImplementedError
    grid[x0 : x1 + 1, y0 : y1 + 1] = val


def toggle(grid: NDArray, region):
    mapping = {0: 1, 1: 0}
    x0, y0 = region[0]
    x1, y1 = region[1]
    for x in range(x0, x1 + 1):
        for y in range(y0, y1 + 1):
            grid[x, y] = mapping[grid[x, y]]


def parte_1():
    for instruction in input:
        terms = instruction.split()
        command, *rest = terms
        match command:
            case "turn":
                operation, coords0_string, _, coords1_string = rest
                coords0 = tuple(int(c) for c in coords0_string.split(","))
                coords1 = tuple(int(c) for c in coords1_string.split(","))
                turn(operation, grid, [coords0, coords1])
            case "toggle":
                coords0_string, _, coords1_string = rest
                coords0 = tuple(int(c) for c in coords0_string.split(","))
                coords1 = tuple(int(c) for c in coords1_string.split(","))
                toggle(grid, [coords0, coords1])
    print(grid.sum())


def adjust_brightness(operation: str, grid: NDArray, region):
    mapping = {"on": 1, "off": -1, "toggle": 2}
    x0, y0 = region[0]
    x1, y1 = region[1]
    for x in range(x0, x1 + 1):
        for y in range(y0, y1 + 1):
            grid[x, y] = max(0, mapping[operation] + grid[x, y])


def parte_2():
    for instruction in input:
        terms = instruction.split()
        command, *rest = terms
        match command:
            case "turn":
                operation, coords0_string, _, coords1_string = rest
                coords0 = tuple(int(c) for c in coords0_string.split(","))
                coords1 = tuple(int(c) for c in coords1_string.split(","))
                adjust_brightness(operation, grid, [coords0, coords1])
            case "toggle":
                coords0_string, _, coords1_string = rest
                coords0 = tuple(int(c) for c in coords0_string.split(","))
                coords1 = tuple(int(c) for c in coords1_string.split(","))
                adjust_brightness("toggle", grid, [coords0, coords1])
    print(grid.sum())


if __name__ == "__main__":
    parte_1()
    parte_2()
