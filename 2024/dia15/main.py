from pprint import pp
import re

# input = open("./mock_1_input.txt", "r").read().strip()
# input = open("./mock_1_input.txt", "r").read().strip()
input = open("./input.txt", "r").read().strip()
# print(input)

map, commands = input.split("\n\n")
# print(map)
# print(commands)

commands = re.sub(r"\n", "", commands)
# print(commands)


def make_grid(input):
    row_strings = input.split()
    max_x = 0
    max_y = 0
    grid = {}
    for y, row_string in enumerate(row_strings):
        for x, string in enumerate(row_string):
            max_x = max(max_x, x)
            max_y = max(max_y, y)
            grid[(x, y)] = string
    return (grid, (max_x, max_y))


def print_grid(grid, grid_size: tuple[int, int]):
    max_x, max_y = grid_size
    lines = [[f"{grid[(x,y)]:1}" for x in range(max_x + 1)] for y in range(max_y + 1)]
    for line in lines:
        print("".join(line))


grid, grid_size = make_grid(map)
# print_grid(grid, grid_size)


def find_char(grid, char=""):
    for coord in grid:
        if grid[coord] == char:
            return coord


initial_pos = find_char(grid, "@")

direction_mapping = {
    "^": (0, -1),
    ">": (1, 0),
    "v": (0, 1),
    "<": (-1, 0),
}


def look_ahead(pos, direction, grid):
    if grid[pos] in [".", "#"]:
        return grid[pos]
    else:
        x, y = pos
        dx, dy = direction_mapping[direction]
        new_pos = (x + dx, y + dy)
        return grid[pos] + look_ahead(new_pos, direction, grid)


def run_commands(grid, pos, commands):
    for command in commands:
        ahead_string = look_ahead(pos, command, grid)
        if ahead_string[-1] == ".":
            x, y = pos
            dx, dy = direction_mapping[command]
            i = 1
            for char in ahead_string[1:-1]:
                i += 1
                tmp_new_pos = (x + i * dx, y + i * dy)
                grid[tmp_new_pos] = "O"
            new_pos = (x + dx, y + dy)
            grid[pos] = "."
            grid[new_pos] = "@"
            pos = new_pos


def calc_GPS(grid):
    token_x = 1
    token_y = 100
    total = 0
    for coord in grid:
        if grid[coord] == "O":
            x, y = coord
            total += x * token_x + y * token_y
    return total


# a = look_ahead(initial_pos, ">", grid)
# print(a, a[-1])


def parte_1() -> None:
    run_commands(grid, initial_pos, commands)
    # print_grid(grid, grid_size)
    print(calc_GPS(grid))


def parte_2() -> None: ...


if __name__ == "__main__":
    parte_1()
    # parte_2()
