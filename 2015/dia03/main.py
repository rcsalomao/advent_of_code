input = open("input.txt", "r").read().strip()
# print(input)

instruction_map = {
    ">": (1, 0),
    "<": (-1, 0),
    "^": (0, 1),
    "v": (0, -1),
}


def parte_1():
    position = (0, 0)
    positions = set([position])
    for instruction in input:
        dx, dy = instruction_map[instruction]
        x, y = position
        position = (x + dx, y + dy)
        positions.add(position)
    print(len(positions))


def parte_2():
    santa_position = (0, 0)
    robo_santa_position = (0, 0)
    positions = set([santa_position, robo_santa_position])
    flag = True
    for instruction in input:
        dx, dy = instruction_map[instruction]
        if flag:
            x, y = santa_position
            santa_position = (x + dx, y + dy)
            positions.add(santa_position)
        else:
            x, y = robo_santa_position
            robo_santa_position = (x + dx, y + dy)
            positions.add(robo_santa_position)
        flag = not flag
    print(len(positions))


if __name__ == "__main__":
    parte_1()
    parte_2()
