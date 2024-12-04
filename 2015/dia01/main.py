input = open("input.txt", "r").read().strip()
# print(input)


def parte_1():
    map = {"(": 1, ")": -1}
    floor = 0
    for char in input:
        floor += map[char]
    print(floor)


def parte_2():
    map = {"(": 1, ")": -1}
    floor = 0
    position = 0
    for char in input:
        floor += map[char]
        position += 1
        if floor == -1:
            break
    print(position)


if __name__ == "__main__":
    parte_1()
    parte_2()
