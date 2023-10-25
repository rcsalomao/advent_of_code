import string
import networkx as nx

input = """
Sabqponm
abcryxxl
accszExk
acctuvwj
abdefghi
"""
input = input.strip().split("\n")
input = open("dia12_input.txt", "r").read().strip().split("\n")
# print(input)


class Grid(object):
    def __init__(self, data):
        n_rows = len(data)
        n_cols = len(data[0])
        grid_size = n_rows * n_cols
        grid = [None] * grid_size
        index_coords_map = [(None, None)] * grid_size
        for i in range(n_rows):
            for j, v in enumerate(data[i]):
                grid[i * n_cols + j] = v
                index_coords_map[i * n_cols + j] = (i, j)
        self.grid_size = grid_size
        self.grid = grid
        self.index_coords_map = index_coords_map
        self.n_rows = n_rows
        self.n_cols = n_cols

    def get_index_from_coord(self, i, j):
        return i * self.n_cols + j

    def get_coord_from_index(self, index):
        return self.index_coords_map[index]

    def get_value(self, i, j):
        return self.grid[i * self.n_cols + j]

    def set_value(self, i, j, value):
        self.grid[i * self.n_cols + j] = value

    def get_neighbours(self, i, j):
        points = []
        for k in [-1, +1]:
            m = i + k
            if m >= 0 and m < self.n_rows:
                points.append((m, j, self.get_value(m, j)))
        for l in [-1, +1]:
            n = j + l
            if n >= 0 and n < self.n_cols:
                points.append((i, n, self.get_value(i, n)))
        return points

    def get_index_from_first_value(self, value):
        return self.grid.index(value)

    def get_coords_from_first_value(self, value):
        return self.index_coords_map[self.grid.index(value)]

    def get_all_coords_values(self):
        res = [(None, None, None)] * self.grid_size
        for i in range(self.grid_size):
            res[i] = (*self.index_coords_map[i], self.grid[i])
        return res

    def get_all_indexes_from_value(self, value):
        return [idx for idx, val in enumerate(self.grid) if val == value]

    def get_all_coords_from_value(self, value):
        return [
            self.index_coords_map[idx]
            for idx, val in enumerate(self.grid)
            if val == value
        ]


def construct_graph(grid: Grid, alphabet_index: dict):
    graph = nx.DiGraph()
    for item in grid.get_all_coords_values():
        i, j, value = item
        item_value = alphabet_index[value]
        neighbours = grid.get_neighbours(i, j)
        neighbours_list = []
        for neighbour in neighbours:
            i_n, j_n, value_n = neighbour
            neighbour_value = alphabet_index[value_n]
            delta_value = neighbour_value - item_value
            if delta_value <= 1:
                neighbours_list.append(neighbour)
        for neighbour in neighbours_list:
            i_n, j_n, value_n = neighbour
            graph.add_edge(
                grid.get_index_from_coord(i, j), grid.get_index_from_coord(i_n, j_n)
            )
    return graph


alphabet_index = {v: k for k, v in enumerate(string.ascii_lowercase)}
alphabet_index["S"] = -1
alphabet_index["E"] = 26
grid = Grid(input)
graph = construct_graph(grid, alphabet_index)
start = grid.get_coords_from_first_value("S")
end = grid.get_coords_from_first_value("E")

# Parte 1
print(
    nx.astar_path_length(
        graph, grid.get_index_from_coord(*start), grid.get_index_from_coord(*end)
    )
)

# Parte 2
res = []
for idx in grid.get_all_indexes_from_value("a"):
    try:
        res.append(nx.astar_path_length(graph, idx, grid.get_index_from_coord(*end)))
    except Exception:
        pass
print(min(res))
