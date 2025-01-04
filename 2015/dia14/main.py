import itertools
import math
import re
from pprint import pp

input = open("input.txt", "r").read().strip()
# input = open("mock_input.txt", "r").read().strip()
# print(input)

reeinder_data: dict[str, tuple[int, ...]] = {}
for line in input.splitlines():
    times = tuple(int(a) for a in re.findall(r"\d+", line))
    reeinder_name: str = re.match(r"(\w+)\W", line).group(0)  # type: ignore
    reeinder_data |= {reeinder_name: times}
# pp(reeinder_data)


def calc_total_travel_distance(velocity, travel_time, rest_time, total_time):
    cycle_time = travel_time + rest_time
    cycles = total_time // cycle_time
    a = total_time % cycle_time
    return (cycles * travel_time + min(travel_time, a)) * velocity


def parte_1():
    total_time = 2503
    max_dist = 0
    for reeinder_name, times in reeinder_data.items():
        velocity, travel_time, rest_time = times
        dist = calc_total_travel_distance(velocity, travel_time, rest_time, total_time)
        if dist > max_dist:
            max_dist = dist
    print(max_dist)


def parte_2(): ...


if __name__ == "__main__":
    parte_1()
    # parte_2()
