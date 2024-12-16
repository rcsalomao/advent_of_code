import networkx as nx
from pprint import pp
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
            grid[(x, y)] = int(string)
    return (grid, (max_x, max_y))


def print_grid(grid, grid_size: tuple[int, int]):
    max_x, max_y = grid_size
    lines = [[f"{grid[(x,y)]:1}" for x in range(max_x + 1)] for y in range(max_y + 1)]
    for line in lines:
        print("".join(line))


neighbours_map = {
    "up": (0, -1),
    "down": (0, 1),
    "left": (-1, 0),
    "right": (1, 0),
}


def calc_graph(grid):
    graph = nx.DiGraph()
    max_x, max_y = grid_size
    for coord in grid:
        for neighbour in neighbours_map:
            try:
                dx, dy = neighbours_map[neighbour]
                x, y = coord
                value = grid[coord]
                neighbour_coord = (x + dx, y + dy)
                neighbour_value = grid[neighbour_coord]
                if neighbour_value - value == 1:
                    graph.add_edge(coord, neighbour_coord)
            except KeyError:
                pass
    return graph


grid, grid_size = make_grid(input)
grid_graph = calc_graph(grid)

starting_coords_base = {}
ending_coords = set()
for coord, value in grid.items():
    if value == 0:
        starting_coords_base[coord] = 0
    elif value == 9:
        ending_coords.add(coord)


def parte_1():
    starting_coords = deepcopy(starting_coords_base)
    for starting_coord in starting_coords:
        for ending_coord in ending_coords:
            try:
                nx.shortest_path(grid_graph, source=starting_coord, target=ending_coord)
                starting_coords[starting_coord] += 1
            except Exception:
                pass
    # pp(starting_coords)
    print(sum(starting_coords.values()))


def parte_2():
    starting_coords = deepcopy(starting_coords_base)
    for starting_coord in starting_coords:
        for ending_coord in ending_coords:
            try:
                n_paths = len(
                    list(
                        nx.all_simple_paths(
                            grid_graph, source=starting_coord, target=ending_coord
                        )
                    )
                )
                starting_coords[starting_coord] += n_paths
            except Exception:
                pass
    # pp(starting_coords)
    print(sum(starting_coords.values()))


if __name__ == "__main__":
    # parte_1()
    parte_2()
