import numpy as np

lines = [line.split() for line in open("input.txt", "r").readlines()]
# lines = [line.split() for line in open("mock_input.txt", "r").readlines()]
reports = [np.array([int(j) for j in report]) for report in lines]


def calc_list_diff(list_a, list_b):
    res = 0
    for i in range(len(list_a)):
        res += abs(list_a[i] - list_b[i])
    return res


def check_if_sorted(lista):
    return (
        calc_list_diff(lista, sorted(lista)) == 0
        or calc_list_diff(lista, sorted(lista, reverse=True)) == 0
    )


def check_level_diff(lista, min_diff=1, max_diff=3):
    n = len(lista)
    for i in range(n - 1):
        diff = abs(lista[i] - lista[i + 1])
        if (diff < min_diff) or (max_diff < diff):
            return False
    return True


# ====== PARTE 1 ======

final_condition = []
for report in reports:
    condition_1 = check_if_sorted(report)
    condition_2 = check_level_diff(report)
    final_condition.append(condition_1 and condition_2)

print(sum(final_condition))

# ====== PARTE 2 ======
for i in range(len(reports)):
    if final_condition[i]:
        continue
    n_levels = len(reports[i])
    for j in range(n_levels):
        n_indexes = list(range(0, j)) + list(range(j + 1, n_levels))
        new_report = reports[i][n_indexes]
        condition_1 = check_if_sorted(new_report)
        condition_2 = check_level_diff(new_report)
        final_condition[i] = condition_1 and condition_2
        if final_condition[i]:
            break

# print(final_condition)
print(sum(final_condition))
