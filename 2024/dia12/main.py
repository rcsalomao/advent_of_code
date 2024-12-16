import networkx as nx
from pprint import pp
from uuid import uuid4, UUID

# input = open("./mock_1_input.txt", "r").read().strip()
# input = open("./mock_2_input.txt", "r").read().strip()
# input = open("./mock_3_input.txt", "r").read().strip()
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


neighbours_map = {
    "up": (0, -1),
    "right": (1, 0),
    "down": (0, 1),
    "left": (-1, 0),
}

lookup_map = {
    "up": (0, -1),
    "ur": (1, -1),
    "rt": (1, 0),
    "rd": (1, 1),
    "dn": (0, 1),
    "dl": (-1, 1),
    "lt": (-1, 0),
    "lu": (-1, -1),
}

corners_patterns = [
    #  outer corners
    {
        "up": "X",
        "ur": "X",
        "rt": "X",
    },
    {
        "rt": "X",
        "rd": "X",
        "dn": "X",
    },
    {
        "dn": "X",
        "dl": "X",
        "lt": "X",
    },
    {
        "up": "X",
        "lt": "X",
        "lu": "X",
    },
    #  inner corners
    {
        "up": "O",
        "ur": "X",
        "rt": "O",
    },
    {
        "rt": "O",
        "rd": "X",
        "dn": "O",
    },
    {
        "dn": "O",
        "dl": "X",
        "lt": "O",
    },
    {
        "up": "O",
        "lt": "O",
        "lu": "X",
    },
    #  mobius corners
    {
        "up": "X",
        "ur": "O",
        "rt": "X",
    },
    {
        "rt": "X",
        "rd": "O",
        "dn": "X",
    },
    {
        "dn": "X",
        "dl": "O",
        "lt": "X",
    },
    {
        "up": "X",
        "lt": "X",
        "lu": "O",
    },
]

grid, grid_size = make_grid(input)
# print_grid(grid, grid_size)


def calc_graph(grid):
    graph = nx.Graph()
    for coord in grid:
        x, y = coord
        plant_type = grid[coord]
        graph.add_node(coord)
        for neighbour, dcoord in neighbours_map.items():
            dx, dy = dcoord
            neighbour_coord = (x + dx, y + dy)
            try:
                neighbour_plant_type = grid[neighbour_coord]
                if neighbour_plant_type == plant_type:
                    graph.add_edge(coord, neighbour_coord)
                else:
                    graph.add_edge(coord, uuid4())
            except KeyError:
                graph.add_edge(coord, uuid4())
    return graph


map_graph = calc_graph(grid)


def calc_total_area_and_perimeter(connected_nodes_list):
    total_area = 0
    total_perimeter = 0
    for node in connected_nodes_list:
        match node:
            case tuple():
                total_area += 1
            case UUID():
                total_perimeter += 1
    return (total_area, total_perimeter)


def calc_total_area_and_corners(connected_nodes_list):
    total_area = 0
    total_corners = 0
    for node in connected_nodes_list:
        match node:
            case tuple():
                total_area += 1
                x, y = node
                lookup_pattern = {}
                for dir, dcoord in lookup_map.items():
                    dx, dy = dcoord
                    neighbour_coord = (x + dx, y + dy)
                    if neighbour_coord in connected_nodes_list:
                        lookup_pattern[dir] = "O"
                    else:
                        lookup_pattern[dir] = "X"
                for corner_pattern in corners_patterns:
                    is_corner = True
                    for dir in corner_pattern:
                        if corner_pattern[dir] != lookup_pattern[dir]:
                            is_corner = False
                            break
                    if is_corner:
                        total_corners += 1
            case UUID():
                ...
    return (total_area, total_corners)


def parte(parte_num: int):
    calc_price_parameters_function = (
        calc_total_area_and_perimeter if parte_num == 1 else calc_total_area_and_corners
    )
    visited_nodes = {node: False for node in map_graph.nodes}
    total_cost = 0
    i = 0
    for node in map_graph.nodes:
        if visited_nodes[node]:
            continue
        i += 1
        connected_nodes = nx.node_connected_component(map_graph, node)
        area, parameter = calc_price_parameters_function(connected_nodes)
        total_cost += area * parameter
        for n in connected_nodes:
            visited_nodes[n] = True
    print(total_cost)


if __name__ == "__main__":
    parte(1)
    parte(2)
