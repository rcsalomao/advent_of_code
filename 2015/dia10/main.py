input = open("input.txt", "r").read().strip()
# print(input)


def look_and_say(init_sequence):
    current_char = init_sequence[0]
    n_same_char = 1
    result = ""
    for char in init_sequence[1:]:
        if char == current_char:
            n_same_char += 1
        else:
            result += f"{n_same_char}{current_char}"
            current_char = char
            n_same_char = 1
    if n_same_char:
        result += f"{n_same_char}{current_char}"
    return result


def parte_1():
    r = input
    for _ in range(40):
        r = look_and_say(r)
    print(len(r))


def parte_2():
    r = input
    for _ in range(50):
        r = look_and_say(r)
    print(len(r))


if __name__ == "__main__":
    parte_1()
    parte_2()
