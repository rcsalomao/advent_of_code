import numpy as np


# input = open("./mock_input.txt", "r").read().strip()
input = open("./input.txt", "r").read().strip()
# print(input)


new_input = [int(i) for i in list(input)]


def calc_filesystem(new_input, total_size):
    filesystem = np.full(total_size, None)
    i = 0
    j = 0
    for k, n in enumerate(new_input):
        i += n
        if k % 2 == 0:
            filesystem[j:i] = int(k / 2)
        j = i
    return filesystem


def parte_1():
    total_size = sum(new_input)
    filesystem = calc_filesystem(new_input, total_size)
    j = -1
    for i in range(total_size):
        if i >= (total_size - abs(j)):
            break
        if filesystem[i] is not None:
            continue
        while filesystem[j] is None:
            j -= 1
        filesystem[i] = filesystem[j]
        filesystem[j] = None
    total_score = 0
    for i, n in enumerate(filesystem[filesystem != np.array(None)]):
        total_score += i * n
    print(total_score)


def parte_2():
    total_size = sum(new_input)
    filesystem = calc_filesystem(new_input, total_size)
    n_terms_new_input = len(new_input)
    backwards_sum = 0
    for i in range(n_terms_new_input):
        print(f"i: {i} | total_terms: {n_terms_new_input}")
        j = -(i + 1)
        k = n_terms_new_input + j
        if k % 2 != 0:
            backwards_sum += new_input[k]
            continue
        m = new_input[k]
        backwards_sum += m
        n = total_size - backwards_sum
        for o in range(total_size):
            b = filesystem[o : o + m]
            test = b[b != np.array(None)]
            if len(test) == 0 and o < n:
                filesystem[o : o + m] = filesystem[n : n + m]
                filesystem[n : n + m] = None
                break
            if o > n:
                break
    total_score = 0
    for i, n in enumerate(filesystem):
        if n is not None:
            total_score += i * n
    print(total_score)


if __name__ == "__main__":
    parte_1()
    # parte_2()
