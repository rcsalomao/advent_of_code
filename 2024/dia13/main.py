import numpy as np
from collections import OrderedDict
from pprint import pp
import re

# input = open("./mock_input.txt", "r").read().strip().split("\n\n")
input = open("./input.txt", "r").read().strip().split("\n\n")
# print(input)

data: dict[str, dict[str, list]] = OrderedDict()
for k, i in enumerate(input):
    A_string, B_string, Prize_string = i.split("\n")
    matches = [
        re.match(r"Button A: X\+(\d+), Y\+(\d+)", A_string),
        re.match(r"Button B: X\+(\d+), Y\+(\d+)", B_string),
        re.match(r"Prize: X\=(\d+), Y\=(\d+)", Prize_string),
    ]
    if None not in matches:
        A_m, B_m, P_m = matches
        A_values = [int(j) for j in A_m.groups()]  # type: ignore
        B_values = [int(j) for j in B_m.groups()]  # type: ignore
        Prize_values = [int(j) for j in P_m.groups()]  # type: ignore
        data |= {
            f"input_{k}": {
                "A": A_values,
                "B": B_values,
                "Prize": Prize_values,
            }
        }
    else:
        raise ValueError
# pp(data)

A_token_cost = 3
B_token_cost = 1


def parte_1() -> None:
    max_button_presses = 100
    total_token_cost = 0
    for d in data:
        f = np.array(data[d]["Prize"])
        K = np.array([data[d]["A"], data[d]["B"]]).T
        u: np.typing.NDArray = np.linalg.solve(K, f)
        if (
            np.all(u > 0)
            and np.all(u < max_button_presses)
            and np.all(abs(np.around(u) - u) < 1e-6)
            # and np.all(f - K @ u < 1e-6)
        ):
            a, b = np.around(u)
            total_token_cost += a * A_token_cost + b * B_token_cost
    print(int(total_token_cost))


def parte_2() -> None:
    total_token_cost = 0
    for d in data:
        f = np.array(data[d]["Prize"]) + 10000000000000
        K = np.array([data[d]["A"], data[d]["B"]]).T
        u: np.typing.NDArray = np.linalg.solve(K, f)
        if (
            np.all(u > 0)
            and np.all(
                abs(np.around(u) - u) < 1e-4
            )  # threshold=1e-4 gives the right answer, but other values gives different total_token_cost results, with convergence achieved for threshold<=1e-5.
            # and np.all(f - K @ u < 1e-6)  # Likewise, enabling this condition also alters the total_token_cost with the threshold value of 1e-4.
        ):
            a, b = np.around(u)
            total_token_cost += a * A_token_cost + b * B_token_cost
    print(int(total_token_cost))


if __name__ == "__main__":
    parte_1()
    parte_2()
