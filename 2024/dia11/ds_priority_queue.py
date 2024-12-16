# Bradley N. Miller, David L. Ranum
# Introduction to Data Structures and Algorithms in Python
# Copyright 2005

import unittest

# This implementation of binary heap takes key value pairs,
# we will assume that the keys are all comparable

# This implementation has been modified by rcsalomao


class PriorityQueue:
    def __init__(self):
        self.heap_array = [None]
        self.current_size = 0

    def build_heap(self, alist: list[tuple[int, any]]):
        n_alist = len(alist)
        self.current_size = n_alist
        self.heap_array = [None]
        for i in alist:
            self.heap_array.append(i)
        i = n_alist // 2
        while i > 0:
            self.perc_down(i)
            i -= 1

    def min_child(self, i):
        if i * 2 > self.current_size:
            return -1
        else:
            if i * 2 + 1 > self.current_size:
                return i * 2
            else:
                if self.heap_array[i * 2][0] < self.heap_array[i * 2 + 1][0]:
                    return i * 2
                else:
                    return i * 2 + 1

    def perc_down(self, i):
        while (i * 2) <= self.current_size:
            mc = self.min_child(i)
            if self.heap_array[i][0] > self.heap_array[mc][0]:
                tmp = self.heap_array[i]
                self.heap_array[i] = self.heap_array[mc]
                self.heap_array[mc] = tmp
            i = mc

    def perc_up(self, i):
        while i // 2 > 0:
            if self.heap_array[i][0] < self.heap_array[i // 2][0]:
                tmp = self.heap_array[i // 2]
                self.heap_array[i // 2] = self.heap_array[i]
                self.heap_array[i] = tmp
            i = i // 2

    def add(self, priority_key_value_tuple: tuple[int, any]):
        self.heap_array.append(priority_key_value_tuple)
        self.current_size += 1
        self.perc_up(self.current_size)

    def is_empty(self):
        if self.current_size == 0:
            return True
        else:
            return False

    def pop_min(self):
        if self.is_empty():
            return None
        retval = self.heap_array[1][1]
        self.heap_array[1] = self.heap_array[self.current_size]
        self.current_size -= 1
        self.heap_array.pop()
        self.perc_down(1)
        return retval

    def get_node(self, value):
        for i in range(1, self.current_size + 1):
            if self.heap_array[i][1] is value:
                return self.heap_array[i]
        return None

    def peek(self):
        return self.heap_array[1][1]

    def decrease_key(self, new_priority_key, value):
        heap_array_i = None
        for i in range(1, self.current_size + 1):
            if self.heap_array[i][1] is value:
                assert new_priority_key <= self.heap_array[i][0]
                heap_array_i = i
                break
        if heap_array_i:
            self.heap_array[heap_array_i] = (
                new_priority_key,
                self.heap_array[heap_array_i][1],
            )
            self.perc_up(heap_array_i)

    def increase_key(self, new_priority_key, value):
        heap_array_i = None
        for i in range(1, self.current_size + 1):
            if self.heap_array[i][1] is value:
                assert new_priority_key >= self.heap_array[i][0]
                heap_array_i = i
                break
        if heap_array_i:
            self.heap_array[heap_array_i] = (
                new_priority_key,
                self.heap_array[heap_array_i][1],
            )
            self.perc_down(heap_array_i)

    def __contains__(self, value):
        for i in range(1, self.current_size + 1):
            if self.heap_array[i][1] is value:
                return True
        return False


class TestBinHeap(unittest.TestCase):
    def setUp(self):
        self.the_heap = PriorityQueue()
        self.the_heap.add((3, "y"))
        self.the_heap.add((5, "z"))
        self.the_heap.add((2, "x"))
        self.the_heap.add((6, "a"))
        self.the_heap.add((4, "d"))

    def testInsert(self):
        assert self.the_heap.current_size == 5

    def testPopMin(self):
        assert self.the_heap.pop_min() == "x"
        assert self.the_heap.pop_min() == "y"
        [self.the_heap.pop_min() for _ in range(self.the_heap.current_size)]
        assert self.the_heap.pop_min() is None

    def testPeek(self):
        assert self.the_heap.peek() == "x"

    def testDecreaseKey(self):
        self.the_heap.decrease_key(1, "d")
        assert self.the_heap.pop_min() == "d"

    def testIncreaseKey(self):
        self.the_heap.increase_key(5, "x")
        assert [self.the_heap.pop_min() for _ in range(2)] == ["y", "d"]

    def testContains(self):
        assert "x" in self.the_heap
        assert "f" not in self.the_heap

    def testGetValue(self):
        assert self.the_heap.get_node("z") == (5, "z")
        assert self.the_heap.get_node("g") is None


if __name__ == "__main__":
    unittest.main()
