# from pprint import pp
from ds_linked_list import LinkedList
from copy import copy

# input = [int(i) for i in open("./mock_input.txt", "r").read().strip().split()]
input = [int(i) for i in open("./input.txt", "r").read().strip().split()]
# print(input)


def blink_with_ll(ll: LinkedList):
    ll.set_current_head()
    for _ in range(ll.size):
        stone = ll.current.item
        if stone == 0:
            ll.current.item = 1
        elif len(str(ll.current.item)) % 2 == 0:
            number_length = len(str(ll.current.item))
            number_char = str(ll.current.item)
            left = number_char[0 : int(number_length / 2)]
            right = number_char[int(number_length / 2) :]
            ll.insert_at_current(int(right))
            ll.insert_at_current(int(left))
            ll.next(2)
            ll.pop_at_current()
            ll.prev(1)
        else:
            ll.current.item *= 2024
        ll.next()


def parte_1():
    ll = LinkedList()
    [ll.append(item) for item in input]
    for i in range(25):
        # print(f"iter: {i}")
        blink_with_ll(ll)
    print(ll.size)


def set_or_increment_value(hash_map, key, quantity):
    try:
        hash_map[key] += quantity
    except KeyError:
        hash_map[key] = quantity


def blink_with_hash_map(stone_pool: dict[int, int]):
    sp = copy(stone_pool)
    for stone, quantity in sp.items():
        if stone == 0:
            set_or_increment_value(stone_pool, 1, quantity)
            stone_pool[0] -= quantity
        elif len(str(stone)) % 2 == 0:
            number_string = str(stone)
            number_length = len(number_string)
            left = int(number_string[0 : number_length // 2])
            right = int(number_string[number_length // 2 :])
            set_or_increment_value(stone_pool, left, quantity)
            set_or_increment_value(stone_pool, right, quantity)
            stone_pool[stone] -= quantity
        else:
            set_or_increment_value(stone_pool, stone * 2024, quantity)
            stone_pool[stone] -= quantity
    return sp


def parte_2():
    stone_pool = {}
    for stone in input:
        set_or_increment_value(stone_pool, stone, 1)
    # pp(stone_pool)
    for i in range(75):
        blink_with_hash_map(stone_pool)
    # pp(stone_pool)
    print(sum(stone_pool.values()))


if __name__ == "__main__":
    # parte_1()
    parte_2()
