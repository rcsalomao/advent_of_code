import numpy as np
import networkx as nx

input = open("dia18_input.txt", "r").read().strip().split("\n")

droplets = []
for i in input:
    x, y, z = i.split(",")
    droplets.append((int(x) + 1, int(y) + 1, int(z) + 1))


# Parte 1
grid = np.full((25, 25, 25), False)
for droplet in droplets:
    x, y, z = droplet
    grid[x, y, z] = True

covered_sides = []
for droplet in droplets:
    x, y, z = droplet
    n_cover_sides = 0
    test_positions = [
        (x - 1, y, z),
        (x + 1, y, z),
        (x, y - 1, z),
        (x, y + 1, z),
        (x, y, z - 1),
        (x, y, z + 1),
    ]
    for position in test_positions:
        x_t, y_t, z_t = position
        if grid[x_t, y_t, z_t]:
            n_cover_sides += 1
    covered_sides.append(n_cover_sides)
print(sum([6 - cs for cs in covered_sides]))


# Parte 2
DG = nx.Graph()
for i in range(24):
    for j in range(24):
        for k in range(24):
            try_neighbours = [
                (i - 1, j, k),
                (i + 1, j, k),
                (i, j - 1, k),
                (i, j + 1, k),
                (i, j, k - 1),
                (i, j, k + 1),
            ]
            for neighb in try_neighbours:
                x, y, z = neighb
                if not grid[x, y, z] and not grid[i, j, k]:
                    DG.add_edge((i, j, k), (x, y, z))

air = set([ed[1] for ed in nx.bfs_edges(DG, (0, 0, 0))] + [(0, 0, 0)])

n_external_face = 0
for i in range(24):
    for j in range(24):
        for k in range(24):
            if grid[i, j, k]:
                try_neighbours = [
                    (i - 1, j, k),
                    (i + 1, j, k),
                    (i, j - 1, k),
                    (i, j + 1, k),
                    (i, j, k - 1),
                    (i, j, k + 1),
                ]
                for neighb in try_neighbours:
                    if neighb in air:
                        n_external_face += 1
print(n_external_face)
