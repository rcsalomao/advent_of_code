from dataclasses import dataclass
from itertools import permutations
from pprint import pp

from numpy import inf

# input = open("../mock_input.txt", "r").read().splitlines()
input = open("../input.txt", "r").read().splitlines()
# print(input)

input_data = [string.split(" = ") for string in input]
# print(input_data)

data = {}
for id in input_data:
    source, dest = id[0].split(" to ")
    data[(source, dest)] = int(id[1])
    data[(dest, source)] = int(id[1])
# pp(data)
locations = set()
for d in data:
    for i in d:
        locations.add(i)
# print(locations)

permutacoes = list(permutations(locations))
print(len(permutacoes))


@dataclass
class TravelData:
    travel_dist: int
    travel_path: set


def parte_1():
    td = TravelData(inf, set())
    for p in permutacoes:
        travel_dist = 0
        for i in range(len(p) - 1):
            travel_dist += data[(p[i], p[i + 1])]
        if travel_dist <= td.travel_dist:
            td.travel_dist = travel_dist
            td.travel_path = p
    # print(td.travel_path)
    print(td.travel_dist)


def parte_2():
    td = TravelData(0, set())
    for p in permutacoes:
        travel_dist = 0
        for i in range(len(p) - 1):
            travel_dist += data[(p[i], p[i + 1])]
        if travel_dist >= td.travel_dist:
            td.travel_dist = travel_dist
            td.travel_path = p
    # print(td.travel_path)
    print(td.travel_dist)


if __name__ == "__main__":
    parte_1()
    parte_2()
