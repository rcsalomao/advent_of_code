from dataclasses import dataclass
from copy import deepcopy

# input = open("./mock_input.txt", "r").read()
input = open("./input.txt", "r").read()
# print(input)


@dataclass
class Data:
    grid: dict[tuple[int, int], str]
    current_pos: tuple[int, int]
    visited_pos: dict[tuple[int, int], int]
    end_in_loop: bool = False


def make_grid(input):
    row_strings = input.split()
    grid = {}
    for y, row_string in enumerate(row_strings):
        for x, string in enumerate(row_string):
            grid[(x, y)] = string
    return grid


def find_char(grid, char: str):
    for coord in grid:
        if grid[coord] == char:
            return coord


move_mapping = {
    "^": (0, -1),
    ">": (1, 0),
    "v": (0, 1),
    "<": (-1, 0),
}


def look_ahead(data: Data):
    grid = data.grid
    current_pos = data.current_pos
    x, y = current_pos
    char = grid[current_pos]
    dx, dy = move_mapping[char]
    next_pos = (x + dx, y + dy)
    try:
        return grid[next_pos]
    except KeyError:
        return False


def move_forward(data: Data):
    grid = data.grid
    current_pos = data.current_pos
    data.visited_pos[current_pos] += 1
    x, y = current_pos
    char = grid[current_pos]
    dx, dy = move_mapping[char]
    next_pos = (x + dx, y + dy)
    grid[next_pos] = char
    grid[current_pos] = "X"
    data.current_pos = next_pos


def turn_90d_right(data: Data):
    grid = data.grid
    current_pos = data.current_pos
    turn_mapping = {
        "^": ">",
        ">": "v",
        "v": "<",
        "<": "^",
    }
    x, y = current_pos
    char = grid[current_pos]
    grid[current_pos] = turn_mapping[char]


def print_grid(grid):
    max_x = 0
    max_y = 0
    for coord in grid:
        x, y = coord
        max_x = max(max_x, x)
        max_y = max(max_y, y)
    lines = [[f"{grid[(x,y)]:1}" for x in range(max_x + 1)] for y in range(max_y + 1)]
    for line in lines:
        print("".join(line))


def run_simulation(data, check_loop=False, max_visits=40):
    max_iter = 100000
    iter = 0
    while True:
        if iter >= max_iter:
            print(f"Número máximo de iterações alcançado: {iter}")
            break
        iter += 1
        ahead_char = look_ahead(data)
        if not ahead_char:
            break
        elif ahead_char in [".", "X"]:
            move_forward(data)
        elif ahead_char == "#":
            turn_90d_right(data)
        else:
            raise NotImplementedError
        if check_loop and iter % 10 == 0:
            visited_pos_values = [v for v in data.visited_pos.values()]
            if max(visited_pos_values) > max_visits:
                data.end_in_loop = True
                break
    data.visited_pos[data.current_pos] += 1


grid = make_grid(input)
initial_pos = find_char(grid, "^")


def parte_1():
    data = Data(
        deepcopy(grid),
        deepcopy(initial_pos),
        {(coords[0], coords[1]): 0 for coords in grid},
    )
    run_simulation(data)
    data.grid[data.current_pos] = "X"
    total_X = 0
    for v in data.grid.values():
        if v == "X":
            total_X += 1
    print(total_X)


def parte_2():
    data = Data(
        deepcopy(grid),
        deepcopy(initial_pos),
        {(coords[0], coords[1]): 0 for coords in grid},
    )
    run_simulation(data)
    data.grid[data.current_pos] = "X"
    initial_visited_positions = [
        (coords[0], coords[1]) for coords, v in data.grid.items() if v == "X"
    ]
    initial_visited_positions.remove(initial_pos)
    total_loops = 0
    total_tentative_pos = len(initial_visited_positions)
    for i, tentative_pos in enumerate(initial_visited_positions):
        data = Data(
            deepcopy(grid),
            deepcopy(initial_pos),
            {(coords[0], coords[1]): 0 for coords in grid},
        )
        data.grid[tentative_pos] = "#"
        run_simulation(data, check_loop=True, max_visits=10)
        if data.end_in_loop:
            total_loops += 1
        print(
            f"tentative: {i+1}/{total_tentative_pos} | end in loop: {data.end_in_loop} | total loops so far: {total_loops}"
        )
    print(total_loops)


if __name__ == "__main__":
    # parte_1()
    parte_2()
