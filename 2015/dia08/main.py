import re

# input = open("mock_input.txt", "r").read().splitlines()
input = open("input.txt", "r").read().splitlines()
# print(input)


def parte_1():
    a = sum([len(line) for line in input])
    b = sum([len(eval(line)) for line in input])
    print(a - b)


def parte_2():
    a = sum([len(line) for line in input])
    new_lines = []
    for line in input:
        new_line, n = re.subn(r"\\", r"\\\\", line)
        new_line, n = re.subn(r'"', r"\"", new_line)
        new_lines.append(new_line)
    b = sum([len(nl) + 2 for nl in new_lines])
    print(b - a)


if __name__ == "__main__":
    parte_1()
    parte_2()
