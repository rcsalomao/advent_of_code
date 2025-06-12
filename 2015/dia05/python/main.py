import re

input = open("../input.txt", "r").read().strip()
# print(input)


def parte_1():
    pattern_1 = r"[aeiou]"
    pattern_2 = r"(\w)\1"
    pattern_3 = r"ab|cd|pq|xy"
    n_nice_strings = 0
    for string in input.split():
        match_1 = re.findall(pattern_1, string)
        match_2 = re.findall(pattern_2, string)
        match_3 = re.findall(pattern_3, string)
        if len(match_1) > 2 and len(match_2) and not len(match_3):
            n_nice_strings += 1
    print(n_nice_strings)


def parte_2():
    pattern_1 = r"(\w\w)\w*\1"
    pattern_2 = r"(\w)\w\1"
    n_nice_strings = 0
    for string in input.split():
        match_1 = re.findall(pattern_1, string)
        match_2 = re.findall(pattern_2, string)
        if len(match_1) and len(match_2):
            n_nice_strings += 1
    print(n_nice_strings)


if __name__ == "__main__":
    parte_1()
    parte_2()
