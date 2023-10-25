import numpy as np


input = [
    line.split(" -> ")
    for line in open("dia14_input.txt", "r").read().strip().split("\n")
]

walls_coords = []
for wall in input:
    wall_coords = []
    for coord in wall:
        x, y = tuple(coord.split(","))
        wall_coords.append((int(x), int(y)))
    walls_coords.append(wall_coords)


def get_wall_coord_list(walls_coords):
    res = []
    bottom = 0
    for wall_coords in walls_coords:
        for i in range(len(wall_coords)):
            if i == 0:
                continue
            x1, y1 = wall_coords[i - 1]
            x2, y2 = wall_coords[i]
            xrange = range(x1, x2 + 1) if x2 >= x1 else range(x2, x1 + 1)
            yrange = range(y1, y2 + 1) if y2 >= y1 else range(y2, y1 + 1)
            res += [(x, y) for x in xrange for y in yrange]
            bottom = max(bottom, y1, y2)
    return res, bottom


wall_obstacle_list, bottom = get_wall_coord_list(walls_coords)
# print(len(wall_obstacle_list), wall_obstacle_list, bottom)

cave = np.full((250, 1000), ".")
for wall_obstacle in wall_obstacle_list:
    j, i = wall_obstacle
    cave[i, j] = "#"


# Parte 1
def run_simulation_part1(cave, bottom):
    cave = cave.copy()
    x_sand = 0
    y_sand = 0
    n_sand_grains = 0
    keep_running = True
    is_the_sand_moving = False
    while keep_running:
        if is_the_sand_moving:
            if cave[y_sand + 1, x_sand] == ".":
                y_sand += 1
            elif cave[y_sand + 1, x_sand - 1] == ".":
                y_sand += 1
                x_sand -= 1
            elif cave[y_sand + 1, x_sand + 1] == ".":
                y_sand += 1
                x_sand += 1
            else:
                cave[y_sand, x_sand] = "O"
                is_the_sand_moving = False
            if y_sand > bottom:
                break
        else:
            x_sand = 500
            y_sand = 1
            n_sand_grains += 1
            is_the_sand_moving = True
    return n_sand_grains - 1


print(run_simulation_part1(cave, bottom))


# Parte 2
def run_simulation_part2(cave, bottom):
    cave = cave.copy()
    n_sand_grains = 0
    infinite_floor = bottom + 2
    while True:
        x_sand = 500
        y_sand = 0
        n_sand_grains += 1
        while True:
            if y_sand + 1 >= infinite_floor:
                cave[y_sand, x_sand] = "O"
                break
            elif cave[1, 500] == cave[1, 499] == cave[1, 501] == "O":
                return n_sand_grains
            elif cave[y_sand + 1, x_sand] == ".":
                y_sand += 1
            elif cave[y_sand + 1, x_sand - 1] == ".":
                y_sand += 1
                x_sand -= 1
            elif cave[y_sand + 1, x_sand + 1] == ".":
                y_sand += 1
                x_sand += 1
            else:
                cave[y_sand, x_sand] = "O"
                break


print(run_simulation_part2(cave, bottom))
