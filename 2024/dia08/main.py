from copy import deepcopy

# input = open("./mock_input.txt", "r").read().strip()
input = open("./input.txt", "r").read().strip()
# print(input)


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
    for coord in grid:
        x, y = coord
    lines = [[f"{grid[(x,y)]:1}" for x in range(max_x + 1)] for y in range(max_y + 1)]
    for line in lines:
        print("".join(line))


def check_bounds(coord: tuple[int, int], grid_size: tuple[int, int]):
    x, y = coord
    g_x, g_y = grid_size
    check_1 = 0 <= x <= g_x
    check_2 = 0 <= y <= g_y
    return check_1 and check_2


def get_antennas(grid):
    antennas = {}
    for coords, char in grid.items():
        if char != ".":
            try:
                antennas[char].append(coords)
            except KeyError:
                antennas[char] = [(coords)]
    return antennas


def calc_antinodes(node_1_coord, node_2_coord, grid_size, parte: int):
    x1, y1 = node_1_coord
    x2, y2 = node_2_coord
    dx = x2 - x1
    dy = y2 - y1
    antinodes_coords = []
    if parte == 1:
        antinode_1_coord = (x1 - dx, y1 - dy)
        antinode_2_coord = (x2 + dx, y2 + dy)
        for antinode_coord in [antinode_1_coord, antinode_2_coord]:
            if check_bounds(antinode_coord, grid_size):
                antinodes_coords.append(antinode_coord)
    elif parte == 2:
        # backward
        flag = True
        i = 0
        while flag:
            antinode_coord = (x1 - i * dx, y1 - i * dy)
            i += 1
            if check_bounds(antinode_coord, grid_size):
                antinodes_coords.append(antinode_coord)
            else:
                flag = False
        # forward
        flag = True
        i = 0
        while flag:
            antinode_coord = (x2 + i * dx, y2 + i * dy)
            i += 1
            if check_bounds(antinode_coord, grid_size):
                antinodes_coords.append(antinode_coord)
            else:
                flag = False
    else:
        raise NotImplementedError
    return antinodes_coords


grid, grid_size = make_grid(input)
antennas = get_antennas(grid)
# print_grid(grid, grid_size)
# print(antennas)


def parte(parte_num: int, print_antinode_grid: bool = False):
    antinode_grid = deepcopy(grid)
    total_antinodes_set = set()
    for antenna_label, antenna_coords in antennas.items():
        antenna_pairs = set()
        n_antennas = len(antenna_coords)
        for i in range(n_antennas):
            antenna_1 = antenna_coords[i]
            for j in range(n_antennas):
                if i == j or i > j:
                    continue
                antenna_2 = antenna_coords[j]
                antenna_pairs.add((antenna_1, antenna_2))
        for pair_coords in antenna_pairs:
            antinodes_coords = calc_antinodes(*pair_coords, grid_size, parte_num)
            for antinode_coord in antinodes_coords:
                antinode_grid[antinode_coord] = "#"
                total_antinodes_set.add(antinode_coord)
    if print_antinode_grid:
        print_grid(antinode_grid, grid_size)
    print(len(total_antinodes_set))


if __name__ == "__main__":
    parte(1)
    parte(2)
