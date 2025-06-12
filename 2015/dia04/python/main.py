import hashlib

input = open("../input.txt", "r").read().strip()
# print(input)


def find_num(n_zeros, max_tries=100000000):
    zeros = "".join(["0" for _ in range(n_zeros)])
    for num in range(max_tries):
        secret = input + str(num)
        md5_hash = hashlib.md5(secret.encode())
        if md5_hash.hexdigest()[0 : len(zeros)] == zeros:
            break
    return num


def parte_1():
    num = find_num(5)
    print(num)


def parte_2():
    num = find_num(6)
    print(num)


if __name__ == "__main__":
    parte_1()
    parte_2()
