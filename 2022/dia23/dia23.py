import numpy as np

input = """
..............
..............
.......#......
.....###.#....
...#...#.#....
....#...##....
...#.###......
...##.#.##....
....#..#......
..............
..............
..............
"""

input = input.strip().split("\n")
# input = open("dia23_input.txt", "r").read().strip().split("\n")

n = 1
grid_size_i = n * len(input)
grid_size_j = n * len(input[0])
grid = np.full((grid_size_i, grid_size_j), ".")

translate_i = 0
translate_j = 0
i = 0
for line in input:
    j = 0
    for char in line:
        if char != ".":
            grid[i + translate_i, j + translate_j] = char
        j += 1
    i += 1

print(grid.shape)


def maybe_north_proposal(grid, i, j):
    rule1 = [
        (i - 1, j),
        (i - 1, j + 1),
        (i - 1, j - 1),
    ]
    goto_rule1 = True
    for rule in rule1:
        k, l = rule
        if grid[k, l] == "#":
            goto_rule1 = False
    if goto_rule1:
        return (i - 1, j)


def maybe_south_proposal(grid, i, j):
    rule2 = [
        (i + 1, j),
        (i + 1, j + 1),
        (i + 1, j - 1),
    ]
    goto_rule2 = True
    for rule in rule2:
        k, l = rule
        if grid[k, l] == "#":
            goto_rule2 = False
    if goto_rule2:
        return (i + 1, j)


def maybe_west_proposal(grid, i, j):
    rule3 = [
        (i, j - 1),
        (i - 1, j - 1),
        (i + 1, j - 1),
    ]
    goto_rule3 = True
    for rule in rule3:
        k, l = rule
        if grid[k, l] == "#":
            goto_rule3 = False
    if goto_rule3:
        return (i, j - 1)


def maybe_east_proposal(grid, i, j):
    rule4 = [
        (i, j + 1),
        (i - 1, j + 1),
        (i + 1, j + 1),
    ]
    goto_rule4 = True
    for rule in rule4:
        k, l = rule
        if grid[k, l] == "#":
            goto_rule4 = False
    if goto_rule4:
        return (i, j + 1)


def maybe_not_move_proposal(grid, i, j):
    rule0 = [
        (i - 1, j),
        (i + 1, j),
        (i, j - 1),
        (i, j + 1),
        (i - 1, j + 1),
        (i - 1, j - 1),
        (i + 1, j + 1),
        (i + 1, j - 1),
    ]
    goto_rule0 = True
    for rule in rule0:
        k, l = rule
        if grid[k, l] == "#":
            goto_rule0 = False
    if goto_rule0:
        return None


def calc_proposals(proposals, grid, i, j):
    for proposal in proposals:
        p = proposal(grid, i, j)
        if p is None or isinstance(p, tuple):
            return p


def run_rounds(grid):
    i_size, j_size = grid.shape
    proposal_cycle = [
        [
            maybe_not_move_proposal,
            maybe_north_proposal,
            maybe_south_proposal,
            maybe_west_proposal,
            maybe_east_proposal,
        ],
        [
            maybe_not_move_proposal,
            maybe_south_proposal,
            maybe_west_proposal,
            maybe_east_proposal,
            maybe_north_proposal,
        ],
        [
            maybe_not_move_proposal,
            maybe_west_proposal,
            maybe_east_proposal,
            maybe_north_proposal,
            maybe_south_proposal,
        ],
        [
            maybe_not_move_proposal,
            maybe_east_proposal,
            maybe_north_proposal,
            maybe_south_proposal,
            maybe_west_proposal,
        ],
    ]
    mark_for_removal = []
    proposals_elfs = {}
    i = 0
    while i < 1:
        for i in range(i_size):
            for j in range(j_size):
                if grid[i, j] == "#":
                    proposal = calc_proposals(proposal_cycle[i % 4], grid, i, j)
                    if proposal is not None:
                        if proposals_elfs.get(proposal):
                            mark_for_removal.append(proposal)
                        else:
                            proposals_elfs[proposal] = (i, j)
        for proposal in mark_for_removal:
            del proposals_elfs[proposal]
        for proposal in proposals_elfs:
            i, j = proposals_elfs[proposal]  # elf
            grid[i, j] = "."
            k, l = proposal
            grid[k, l] = "#"
        i += 1


run_rounds(grid)

i_size, j_size = grid.shape
for i in range(i_size):
    for j in range(j_size):
        print(grid[i, j], end="")
    print()
