import re
from functools import cmp_to_key

input = open("input.txt", "r").read().strip().split("\n\n")
input_parte1 = [tuple(pair.split("\n")) for pair in input]
input_parte2 = []
for pair in input:
    input_parte2 += pair.split("\n")


def build_string_list(string):
    string_list = list(re.findall(r"^\[(.*)\]$", string)[0])
    res = []
    is_there_a_list_in_there = False
    list_char_appending = 0
    list_char = ""
    number_char = ""
    i = 0
    string_list_size = len(string_list)
    for char in string_list:
        i += 1
        if char == "]":
            list_char_appending -= 1
            list_char += char
            if not list_char_appending:
                res.append(list_char)
                list_char = ""
        elif char == "[":
            is_there_a_list_in_there = True
            list_char_appending += 1
            list_char += char
        elif list_char_appending:
            list_char += char
        elif char != ",":
            number_char += char
        elif char in [",", "]"]:
            if number_char != "":
                res.append(int(number_char))
            number_char = ""
        if i == string_list_size:
            if number_char != "":
                res.append(int(number_char))
    if is_there_a_list_in_there:
        for i in range(len(res)):
            if re.match(r"^\[(.*)\]$", str(res[i])):
                res[i] = build_string_list(res[i])
    return res


def process_comparison(left, right):
    if isinstance(left, int) and isinstance(right, int):
        return left - right
    if isinstance(left, list) and isinstance(right, list):
        for val in zip(left, right):
            r = process_comparison(val[0], val[1])
            if r != 0:
                return r
        return len(left) - len(right)
    if isinstance(left, list) and isinstance(right, int):
        return process_comparison(left, [right])
    if isinstance(left, int) and isinstance(right, list):
        return process_comparison([left], right)


def run_parte1():
    global input_parte1
    i = 0
    soma = 0
    for input_data in input_parte1:
        i += 1
        left_input, right_input = input_data
        left_list = build_string_list(left_input)
        right_list = build_string_list(right_input)
        test = process_comparison(left_list, right_list) <= 0
        if test:
            soma += i
    print(soma)


def run_parte2():
    global input_parte2
    input_parte2 += ["[[2]]", "[[6]]"]
    input_data_list = [build_string_list(string) for string in input_parte2]
    sorted_input_data_list = sorted(input_data_list, key=cmp_to_key(process_comparison))
    i = 0
    prod = 1
    for packet in sorted_input_data_list:
        i += 1
        if packet in [[[2]], [[6]]]:
            prod *= i
    print(prod)


run_parte1()
run_parte2()
