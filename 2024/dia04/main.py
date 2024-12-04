# input = open("./mock_input.txt", "r").read()
input = open("./input.txt", "r").read()
# print(input)


def make_grid(input):
    row_strings = input.split()
    grid = {}
    for i, row_string in enumerate(row_strings):
        for j, string in enumerate(row_string):
            grid[(i, j)] = string
    return grid


def parte_1():
    grid = make_grid(input)
    word_directions = {
        "hd": [(0, i) for i in range(4)],
        "he": [(0, -i) for i in range(4)],
        "vc": [(i, 0) for i in range(4)],
        "vb": [(-i, 0) for i in range(4)],
        "dc": [(i, i) for i in range(4)],
        "db": [(i, -i) for i in range(4)],
        "ec": [(-i, i) for i in range(4)],
        "eb": [(-i, -i) for i in range(4)],
    }
    total_xmas = 0
    for coord, letter in grid.items():
        if letter != "X":
            continue
        x, y = coord
        for direction, letter_coords in word_directions.items():
            word = "".join(
                filter(
                    lambda x: x is not None,
                    [grid.get((x + i, y + j)) for (i, j) in letter_coords],
                )
            )
            if word == "XMAS":
                total_xmas += 1
    print(total_xmas)


def parte_2():
    grid = make_grid(input)
    word_directions = {
        "diag_1": [(i, -i) for i in range(-1, 2)],
        "diag_2": [(i, i) for i in range(-1, 2)],
    }
    total_xmas = 0
    for coord, letter in grid.items():
        if letter != "A":
            continue
        x, y = coord
        words = {}
        for direction, letter_coords in word_directions.items():
            word = "".join(
                filter(
                    lambda x: x is not None,
                    [grid.get((x + i, y + j)) for (i, j) in letter_coords],
                )
            )
            words[direction] = word
        diag_1, diag_2 = words["diag_1"], words["diag_2"]
        if ("MAS" in [diag_1, diag_1[::-1]]) and ("MAS" in [diag_2, diag_2[::-1]]):
            total_xmas += 1
    print(total_xmas)


if __name__ == "__main__":
    parte_1()
    parte_2()
