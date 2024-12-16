from pprint import pp
from functools import lru_cache

# input = open("mock_input.txt", "r").read().splitlines()
input = open("input.txt", "r").read().splitlines()
# print(input)

input_data = {}
for line in input:
    cmd, key = line.split(" -> ")
    input_data[key.strip()] = cmd
# pp(input_data)


@lru_cache()
def calc_value(key):
    try:
        return int(key)
    except ValueError:
        ...
    cmd = input_data[key].split(" ")
    if "NOT" in cmd:
        return ~calc_value(cmd[1])
    elif "AND" in cmd:
        return calc_value(cmd[0]) & calc_value(cmd[2])
    elif "OR" in cmd:
        return calc_value(cmd[0]) | calc_value(cmd[2])
    elif "LSHIFT" in cmd:
        return calc_value(cmd[0]) << calc_value(cmd[2])
    elif "RSHIFT" in cmd:
        return calc_value(cmd[0]) >> calc_value(cmd[2])
    else:
        return calc_value(cmd[0])


def parte_1():
    calc_value.cache_clear()
    print(calc_value("a") & 0xFFFF)


def parte_2():
    calc_value.cache_clear()
    input_data["b"] = str(calc_value("a"))
    calc_value.cache_clear()
    print(calc_value("a") & 0xFFFF)


if __name__ == "__main__":
    parte_1()
    parte_2()
