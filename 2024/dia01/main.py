lines = [line.split() for line in open("input.txt", "r").readlines()]
vec_1 = []
vec_2 = []
for v1, v2 in lines:
    vec_1.append(int(v1))
    vec_2.append(int(v2))
n_lines = len(lines)


def parte_1():
    sorted_vec_1 = sorted(vec_1)
    sorted_vec_2 = sorted(vec_2)
    diff = []
    for i in range(n_lines):
        diff.append(abs(sorted_vec_1[i] - sorted_vec_2[i]))
    print(sum(diff))


def parte_2():
    n_ocurrence = []
    for val in vec_1:
        n_ocurrence.append(vec_2.count(val))
    score = []
    for i in range(n_lines):
        score.append(vec_1[i] * n_ocurrence[i])
    print(sum(score))


if __name__ == "__main__":
    parte_1()
    parte_2()
