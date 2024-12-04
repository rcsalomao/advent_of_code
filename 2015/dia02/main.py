from functools import reduce
from operator import mul

input = open("input.txt", "r").read().strip()
print(input)


def parte_1():
    total_square_feet = 0
    for line in input.split():
        dims = [int(d) for d in line.split("x")]
        faces_square_feet = [dims[0] * dims[1], dims[0] * dims[2], dims[1] * dims[2]]
        total_square_feet += 2 * sum(faces_square_feet) + min(faces_square_feet)
    print(total_square_feet)


def parte_2():
    total_feet = 0
    for line in input.split():
        dims = [int(d) for d in line.split("x")]
        sorted_dims = sorted(dims)
        feet_1 = 2 * (sorted_dims[0] + sorted_dims[1])
        feet_2 = reduce(mul, dims, 1)
        total_feet += feet_1 + feet_2
    print(total_feet)


if __name__ == "__main__":
    parte_1()
    parte_2()
