import math
import itertools

# from pprint import pp
import re


input = open("../input.txt", "r").read().strip()
# input = open("mock_input.txt", "r").read().strip()
# print(input)

total_persons = set()
happiness_map: dict[tuple[str, str], int] = dict()
for line in input.splitlines():
    person, other_person = re.match(r"(\w+)\W.*\W(\w+)", line).groups()  # type: ignore
    hap = re.findall(r"\d+", line)[0]
    gain_or_lose = re.findall(r"gain|lose", line)[0]
    match gain_or_lose:
        case "gain":
            hap = hap
        case "lose":
            hap = "-" + hap
    happiness_map |= {(person, other_person): int(hap)}
    total_persons.add(person)


class Person:
    def __init__(self, name):
        self._name = name

    def set_left_person(self, left):
        self._left = left

    def set_right_person(self, right):
        self._right = right

    def calc_happiness(self):
        return (
            happiness_map[(self._name, self._left._name)]
            + happiness_map[(self._name, self._right._name)]
        )

    def __str__(self):
        return self._name


def calc_persons_arrangements(persons_names: set[str]):
    return itertools.permutations(
        [Person(person_name) for person_name in persons_names], len(persons_names)
    )


def calc_total_happiness(persons: list[Person]):
    n = len(persons)
    for i, person in enumerate(persons):
        person.set_left_person(persons[(i - 1) % n])
        person.set_right_person(persons[(i + 1) % n])
    return sum([person.calc_happiness() for person in persons])


def parte_1():
    max_happiness = -math.inf
    persons_arrangements = calc_persons_arrangements(total_persons)
    for person_arrangement in persons_arrangements:
        max_happiness = max(max_happiness, calc_total_happiness(person_arrangement))
    print(max_happiness)


def parte_2():
    global happiness_map
    for person in total_persons:
        happiness_map |= {(person, "Me"): 0, ("Me", person): 0}
    total_persons.add("Me")
    max_happiness = -math.inf
    persons_arrangements = calc_persons_arrangements(total_persons)
    for person_arrangement in persons_arrangements:
        max_happiness = max(max_happiness, calc_total_happiness(person_arrangement))
    print(max_happiness)


if __name__ == "__main__":
    parte_1()
    parte_2()
