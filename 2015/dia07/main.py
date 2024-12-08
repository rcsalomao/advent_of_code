from pprint import pp
import re
from functools import singledispatch

input = open("mock_input.txt", "r").read().splitlines()
# input = open("input.txt", "r").read().splitlines()
# print(input)


@singledispatch
def get_val(val, namespace):
    raise NotImplementedError


@get_val.register
def _(val: str, namespace):
    return namespace[val]


@get_val.register
def _(val: int, namespace):
    return val


def parte_1():
    namespace = {}
    for instruction in input:
        rhs, lhs = instruction.split(" -> ")
        print(lhs, rhs)
        if re.search(r"AND", rhs):
            a, b = rhs.split(" AND ")
            print(a, b)
            # namespace[lhs] = namespace.setdefault(a, int(a)) & namespace.setdefault(
            #     b, int(b)
            # )
            # namespace[lhs] = namespace[a] & namespace[b]
            namespace[lhs] = get_val(a, namespace) & get_val(b, namespace)
        elif re.search(r"OR", rhs):
            a, b = rhs.split(" OR ")
            namespace[lhs] = namespace.setdefault(
                a, bytes(a, encoding="utf-16")
            ) | namespace.setdefault(b, bytes(b, encoding="utf-16"))
        elif re.search(r"LSHIFT", rhs):
            a, b = rhs.split(" LSHIFT ")
            namespace[lhs] = bytes(a) << bytes(b)
        elif re.search(r"RSHIFT", rhs):
            a, b = rhs.split(" RSHIFT ")
            namespace[lhs] = bytes(a) >> bytes(b)
        elif re.search(r"NOT", rhs):
            a, b = rhs.split("NOT ")
            namespace[lhs] = not bytes(b)
        else:
            namespace[lhs] = int(rhs)
    pp(namespace)


def parte_2():
    pass


if __name__ == "__main__":
    parte_1()
    parte_2()
