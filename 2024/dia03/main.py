import re

# input = open("./parte_1_mock_input.txt", "r").read()
# input = open("./parte_2_mock_input.txt", "r").read()
input = open("./input.txt", "r").read()


def make_reduction(matches_to_reduce):
    total = 0
    for match in matches_to_reduce:
        nums = [int(i) for i in re.findall(r"\d+", match)]
        a, b = nums
        total += a * b
    return total


def parte_1():
    pattern = r"mul\(\d+,\d+\)"
    matches = re.findall(pattern, input)
    total = make_reduction(matches)
    print(total)


def parte_2():
    pattern = r"mul\(\d+,\d+\)|do\(\)|don't\(\)"
    matches = re.findall(pattern, input)
    matches_to_reduce = []
    flag = True
    for match in matches:
        if re.match(r"do\(\)", match):
            flag = True
        elif re.match(r"don't\(\)", match):
            flag = False
        else:
            if flag:
                matches_to_reduce.append(match)
    total = make_reduction(matches_to_reduce)
    print(total)


if __name__ == "__main__":
    parte_1()
    parte_2()
