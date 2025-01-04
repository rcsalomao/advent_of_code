import math
from pprint import pp
import networkx as nx

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
    lines = [[f"{grid[(x,y)]:1}" for x in range(max_x + 1)] for y in range(max_y + 1)]
    for line in lines:
        print("".join(line))


grid, grid_size = make_grid(input)
# print_grid(grid, grid_size)


def find_char(grid, char=""):
    for coord in grid:
        if grid[coord] == char:
            return coord


initial_pos = find_char(grid, "S")
final_pos = find_char(grid, "E")
# print(initial_pos, final_pos)

neighbours_map = {
    "^": (0, -1),
    "v": (0, 1),
    "<": (-1, 0),
    ">": (1, 0),
}
orientation_map = {pos: orientation for orientation, pos in neighbours_map.items()}


def calc_graph(grid, grid_size):
    graph = nx.DiGraph()
    max_x, max_y = grid_size
    for coord in grid:
        if grid[coord] == "#":
            continue
        for neighbour in neighbours_map:
            try:
                x, y = coord
                dx, dy = neighbours_map[neighbour]
                neighbour_coord = (x + dx, y + dy)
                if grid[neighbour_coord] in [".", "S", "E"]:
                    graph.add_edge(coord, neighbour_coord)
            except KeyError:
                pass
    return graph


def calc_path_score(path, initial_pos, initial_orientation=">"):
    total = 0
    current_pos = initial_pos
    current_orientation = initial_orientation
    for pos in path:
        cx, cy = current_pos
        nx, ny = pos
        dcoord = (nx - cx, ny - cy)
        orientation = (
            orientation_map[dcoord] if dcoord != (0, 0) else current_orientation
        )
        total += 1
        if current_orientation != orientation:
            total += 1000
        current_pos = pos
        current_orientation = orientation
    total -= 1
    return total


def parte_1() -> None:
    graph = calc_graph(grid, grid_size)
    paths = nx.all_simple_paths(graph, initial_pos, final_pos)
    min_score = math.inf
    i = 0
    for path in paths:
        # print(path)
        score = calc_path_score(path, initial_pos)
        if score < min_score:
            min_score = score
            # min_score_path = path
        i += 1
        print(f"Path: {i}, min score: {min_score}")
    # print(min_score)
    # print(min_score_path)


def parte_2() -> None: ...


if __name__ == "__main__":
    parte_1()
    # parte_2()
