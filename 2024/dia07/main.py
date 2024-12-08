from itertools import product

input = open("./mock_input.txt", "r").read().strip().splitlines()
# input = open("./input.txt", "r").read().strip().splitlines()
# print(input)


def build_equation(operands, operations):
    equation = f"({operands[0]})"
    for i in range(len(operations)):
        if operations[i] in ["+", "*"]:
            equation = f"(int{equation} {operations[i]} int({operands[i+1]}))"
        else:
            equation = f"(str{equation} + str({operands[i+1]}))"
    equation = f"int{equation}"
    return equation


def build_equations(operands, base_operators):
    n_operands = len(operands)
    total_combinations = product(base_operators, repeat=(n_operands - 1))
    equations = []
    for comb in total_combinations:
        equations.append(build_equation(operands, comb))
    return equations


def calc_sum_correct_results(base_operators, simple_progress):
    sum_correct_results = 0
    n_input = len(input)
    for i, line in enumerate(input):
        if simple_progress:
            print(f"Equation [{i+1}/{n_input}]")
        r, o = line.split(": ")
        result = int(r)
        operands = o.split(" ")
        equations = build_equations(operands, base_operators)
        for equation in equations:
            if result == eval(equation):
                sum_correct_results += result
                break
    return sum_correct_results


def parte_1(simple_progress=False):
    soma = calc_sum_correct_results(
        base_operators=("+", "*"), simple_progress=simple_progress
    )
    print(soma)


def parte_2(simple_progress=False):
    soma = calc_sum_correct_results(
        base_operators=("+", "*", "||"), simple_progress=simple_progress
    )
    print(soma)


if __name__ == "__main__":
    parte_1(True)
    parte_2(True)
