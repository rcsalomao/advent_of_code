import re

input = open("input.txt", "r").read().strip().split("\n")

# Parte 1
total_cycles = []
cycle_number = 0
x = 1
total_cycles.append((cycle_number, 0, x))
for instruction in input:
    if re.match(r"addx", instruction):
        number_to_add = int(instruction.split(" ")[1])
        number_cycles = 2
    if re.match(r"noop", instruction):
        number_to_add = 0
        number_cycles = 1
    cycle_number += number_cycles
    x += number_to_add
    total_cycles.append((cycle_number, number_to_add, x))


def interpol_cycle_x(total_cycles, cycle):
    for i in range(len(total_cycles) - 1, -1, -1):
        cycle_number, _, x = total_cycles[i]
        if cycle_number < cycle:
            return x


cycles_of_interest = [20 + 40 * i for i in range(6)]
print(
    sum([cycle * interpol_cycle_x(total_cycles, cycle) for cycle in cycles_of_interest])
)

# Parte 2


def run_CRT(total_cycles):
    cycle = 0
    for i in range(6):
        for j in range(40):
            cycle += 1
            x = interpol_cycle_x(total_cycles, cycle)
            if j in [x - 1, x, x + 1]:
                print("#", end="")
            else:
                print(".", end="")
        print()


run_CRT(total_cycles)
