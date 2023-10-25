input = [
    tuple(move.split(" "))
    for move in open("dia09_input.txt", "r").read().strip().split("\n")
]


def move_head(position, direction):
    direction_map = {
        "U": lambda pos: [pos[0], pos[1] + 1],
        "L": lambda pos: [pos[0] - 1, pos[1]],
        "D": lambda pos: [pos[0], pos[1] - 1],
        "R": lambda pos: [pos[0] + 1, pos[1]],
    }
    return direction_map[direction](position)


def move_tail(position, head_position):
    dx = head_position[0] - position[0]
    dy = head_position[1] - position[1]
    if (abs(dx) > 1) or (abs(dy) > 1):
        position[0] += int(dx / max(1, abs(dx)))
        position[1] += int(dy / max(1, abs(dy)))
    return position


def print_rope(rope, grid_length):
    for i in range(grid_length, 0, -1):
        for j in range(grid_length):
            char = "."
            for k in range(len(rope)):
                x = rope[k][0]
                y = rope[k][1]
                if x == j and y == i:
                    char = k
            print(char, end="")
        print()


# Parte 1
tail_visited_locations = []
rope_length = 2
head = [0, 0]
tail = [0, 0]
rope = [head, tail]
tail_visited_locations.append(tuple(rope[-1]))
for instruction in input:
    direction, move_number = instruction
    for i in range(int(move_number)):
        rope[0] = move_head(rope[0], direction)
        for i in range(1, rope_length):
            rope[i] = move_tail(rope[i], rope[i - 1])
        # print_rope(rope, 3 * rope_length)
        tail_visited_locations.append(tuple(rope[-1]))
print(len(set(tail_visited_locations)))


# Parte 2
tail_visited_locations = []
rope_length = 10
rope = [[0, 0] for i in range(rope_length)]
tail_visited_locations.append(tuple(rope[-1]))
for instruction in input:
    direction, move_number = instruction
    for i in range(int(move_number)):
        rope[0] = move_head(rope[0], direction)
        for i in range(1, rope_length):
            rope[i] = move_tail(rope[i], rope[i - 1])
        # print_rope(rope, 3 * rope_length)
        tail_visited_locations.append(tuple(rope[-1]))
print(len(set(tail_visited_locations)))
