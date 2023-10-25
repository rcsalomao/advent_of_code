import re
from functools import reduce

input = open("dia11_input.txt", "r").read().strip().split("\n\n")


class Monkey(object):
    def __init__(self, input_monkey_string):
        tmp = input_monkey_string.split("\n")
        self.id = int(re.search(r"\w* (\d):", tmp[0]).group(1))
        self.items = [
            int(num)
            for num in re.search(r" +\w* \w*: (.*)", tmp[1]).group(1).split(", ")
        ]
        self.test_function = eval(
            "lambda old: " + re.search(r"  Operation: new = (.*)", tmp[2]).group(1)
        )
        self.test_divisible_num = int(
            re.search(r" +\w*: \w* \w* (\d*)", tmp[3]).group(1)
        )
        self.test_true_num = int(
            re.search(r" +\w* \w*: \w* \w* \w* (\d*)", tmp[4]).group(1)
        )
        self.test_false_num = int(
            re.search(r" +\w* \w*: \w* \w* \w* (\d*)", tmp[5]).group(1)
        )
        self.number_inspections = 0


monkeys = [Monkey(i) for i in input]


def run_round_1(monkeys):
    for monkey in monkeys:
        for item in monkey.items:
            worry_level = int(monkey.test_function(item) / 3)
            if (worry_level % monkey.test_divisible_num) == 0:
                monkeys[monkey.test_true_num].items.append(worry_level)
            else:
                monkeys[monkey.test_false_num].items.append(worry_level)
            monkey.number_inspections += 1
        monkey.items.clear()
    return monkeys


def run_parte1():
    [run_round_1(monkeys) for i in range(20)]
    most_active_monkeys = sorted(
        [monkey.number_inspections for monkey in monkeys], reverse=True
    )[:2]
    print(most_active_monkeys[0] * most_active_monkeys[1])


def run_round_2(monkeys, superMod):
    for monkey in monkeys:
        for item in monkey.items:
            worry_level = monkey.test_function(
                item
            )  # Aqui, de acordo com o enunciado, não é possível ficar mais calmo pela divisão por 3.
            if (worry_level % monkey.test_divisible_num) == 0:
                monkeys[monkey.test_true_num].items.append(
                    worry_level % superMod
                )  # Faz uso de aritmética de módulo com a seguinte igualdade: (x mod k*n) mod n == x mod n. Tomando-se como k*n == n_1 * n_2 * [...] * n_j com j = 1, número de macacos.
            else:
                monkeys[monkey.test_false_num].items.append(
                    worry_level % superMod
                )  # Desta maneira é possível diminuir o "worry_level" sem alterar o resultado.
            monkey.number_inspections += 1
        monkey.items.clear()
    return monkeys


def run_parte2():
    superMod = reduce(
        lambda x, y: x * y, [monkey.test_divisible_num for monkey in monkeys]
    )
    [run_round_2(monkeys, superMod) for i in range(10000)]
    most_active_monkeys = sorted(
        [monkey.number_inspections for monkey in monkeys], reverse=True
    )[:2]
    print(most_active_monkeys[0] * most_active_monkeys[1])


run_parte1()
run_parte2()
