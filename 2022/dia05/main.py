move_directions = open("input.txt", "r").read().strip().split("\n")
move_directions = [
    (int(move_direction[1]), int(move_direction[3]), int(move_direction[5]))
    for move_direction in [
        move_direction.split(" ") for move_direction in move_directions
    ]
]

# Parte 1
crate_stacks = [
    list("GTRW"),
    list("GCHPMSVW"),
    list("CLTSGM"),
    list("JHDMWRF"),
    list("PQLHSWFJ"),
    list("PJDNFMS"),
    list("ZBDFGCSJ"),
    list("RTB"),
    list("HNWLC"),
]


def make_move_parte1(move_direction, crate_stacks):
    move_num, from_num, to_num = move_direction
    for i in range(move_num):
        crate = crate_stacks[from_num - 1].pop()
        crate_stacks[to_num - 1].append(crate)


[make_move_parte1(move_direction, crate_stacks) for move_direction in move_directions]
print("".join([crate_stack[-1] for crate_stack in crate_stacks]))

# Parte 2
crate_stacks = [
    list("GTRW"),
    list("GCHPMSVW"),
    list("CLTSGM"),
    list("JHDMWRF"),
    list("PQLHSWFJ"),
    list("PJDNFMS"),
    list("ZBDFGCSJ"),
    list("RTB"),
    list("HNWLC"),
]


def make_move_parte2(move_direction, crate_stacks):
    move_num, from_num, to_num = move_direction
    crates_substack = crate_stacks[from_num - 1][-move_num:]
    for i in range(move_num):
        crate_stacks[from_num - 1].pop()
    for crate in crates_substack:
        crate_stacks[to_num - 1].append(crate)


[make_move_parte2(move_direction, crate_stacks) for move_direction in move_directions]
print("".join([crate_stack[-1] for crate_stack in crate_stacks]))
