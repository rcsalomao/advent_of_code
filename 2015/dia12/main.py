import json
import re


input = open("input.txt", "r").read().strip()
# print(input)


def parte_1():
    numbers = re.findall(r"-?\d+", input)
    total = 0
    for number in numbers:
        total += int(number)
    print(total)


def parte_2():
    m = json.loads(input)

    def total(i):
        match i:
            case int():
                return i
            case list():
                return sum([total(j) for j in i])
            case dict():
                if "red" in i.values():
                    return 0
                else:
                    return total(list(i.values()))
            case _:
                return 0

    print(total(m))


if __name__ == "__main__":
    parte_1()
    parte_2()
