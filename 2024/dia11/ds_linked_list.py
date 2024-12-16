from ds_priority_queue import PriorityQueue


class LinkedListNode(object):
    def __init__(self, item, next, prev):
        self.item = item
        self.next = self if next is None else next
        self.prev = self if prev is None else prev


class LinkedList(object):
    def __init__(self, item=None, circular=False):
        self.circular = circular
        if item is None:
            self.size = 0
            self.head = None
            self.tail = None
            self.current = None
            self.current_idx = None
        else:
            self.size = 1
            self.head = LinkedListNode(item, None, None)
            self.tail = self.head
            self.current = self.head
            self.current_idx = 0

    def length(self):
        return self.size

    def is_empty(self):
        return self.size == 0

    def set_current_head(self):
        self.current = self.head
        self.current_idx = 0

    def set_current_tail(self):
        self.current = self.tail
        self.current_idx = self.size - 1

    def set_current_nth(self, nth):
        assert 0 <= nth < self.size
        delta_idx = nth - self.current_idx
        if delta_idx > 0:
            self.next(delta_idx)
        elif delta_idx < 0:
            self.prev(abs(delta_idx))
        else:
            pass

    def next(self, n=1):
        assert n >= 0
        if self.size > 1:
            for _ in range(n):
                self.current = self.current.next
                self.current_idx += 1
            if self.circular:
                self.current_idx %= self.size
            else:
                self.current_idx = min(self.current_idx, self.size - 1)

    def prev(self, n=1):
        assert n >= 0
        if self.size > 1:
            for _ in range(n):
                self.current = self.current.prev
                self.current_idx -= 1
            if self.circular:
                self.current_idx %= self.size
            else:
                self.current_idx = max(self.current_idx, 0)

    def append(self, item):
        if self.size == 0:
            self.head = LinkedListNode(item, None, None)
            self.tail = self.head
            self.current = self.head
            self.current_idx = 0
        else:
            if self.circular:
                self.tail = LinkedListNode(item, self.head, self.tail)
                self.head.prev = self.tail
            else:
                self.tail = LinkedListNode(item, None, self.tail)
            self.tail.prev.next = self.tail
        self.size += 1

    def append_left(self, item):
        if self.size == 0:
            self.head = LinkedListNode(item, None, None)
            self.tail = self.head
            self.current = self.head
            self.current_idx = 0
        else:
            if self.circular:
                self.head = LinkedListNode(item, self.head, self.tail)
                self.tail.next = self.head
            else:
                self.head = LinkedListNode(item, self.head, None)
            self.head.next.prev = self.head
            self.current = self.current.prev
        self.size += 1

    def pop(self):
        if self.size == 0:
            return None
        elif self.size == 1:
            item = self.tail.item
            self.head = None
            self.tail = None
            self.current = None
            self.current_idx = None
            self.size -= 1
            return item
        else:
            item = self.tail.item
            if self.current is self.tail:
                self.tail = self.tail.prev
                self.current = self.tail
                self.current_idx -= 1
            else:
                self.tail = self.tail.prev
            if self.circular:
                self.tail.next = self.head
            else:
                self.tail.next = self.tail
            self.size -= 1
            return item

    def pop_left(self):
        if self.size == 0:
            return None
        elif self.size == 1:
            item = self.head.item
            self.head = None
            self.tail = None
            self.current = None
            self.current_idx = None
            self.size -= 1
            return item
        else:
            item = self.head.item
            if self.current is self.head:
                self.head = self.head.next
                self.current = self.head
            else:
                self.head = self.head.next
                self.current_idx -= 1
            if self.circular:
                self.head.prev = self.tail
            else:
                self.head.prev = self.head
            self.size -= 1
            return item

    def pop_at_index(self, index):
        if self.size == 0:
            return None
        elif self.size == 1:
            assert index == 0
            item = self.current.item
            self.head = None
            self.tail = None
            self.current = None
            self.current_idx = None
            self.size -= 1
            return item
        else:
            self.set_current_nth(index)
            item = self.current.item
            if self.current is self.head:
                self.head = self.head.next
                self.current = self.head
                if self.circular:
                    self.head.prev = self.tail
                else:
                    self.head.prev = self.head
            elif self.current is self.tail:
                self.tail = self.tail.prev
                self.current = self.tail
                self.current_idx -= 1
                if self.circular:
                    self.tail.next = self.head
                else:
                    self.tail.next = self.tail
            else:
                self.current.prev.next = self.current.next
                self.current.next.prev = self.current.prev
                self.current = self.current.next
            self.size -= 1
            return item

    def pop_at_current(self):
        return self.pop_at_index(self.current_idx)

    def insert_at_index(self, item, index):
        self.set_current_nth(index)
        if self.current is self.head:
            if self.circular:
                self.current = LinkedListNode(item, self.head, self.tail)
                self.current.next.prev = self.current
                self.head = self.current
            else:
                self.current = LinkedListNode(item, self.head, None)
                self.current.next.prev = self.current
                self.head = self.current
        else:
            self.current = LinkedListNode(item, self.current, self.current.prev)
            self.current.next.prev = self.current
            self.current.prev.next = self.current
        self.size += 1

    def insert_at_current(self, item):
        self.insert_at_index(item, self.current_idx)

    def node_at_index(self, index):
        self.set_current_nth(index)
        return self.current

    def rotate_right(self, n=1):
        assert n >= 0
        if self.size > 1:
            fix_current_tail = self.current is self.tail
            for _ in range(n):
                self.append_left(self.pop())
            if fix_current_tail:
                self.set_current_tail()

    def rotate_left(self, n=1):
        assert n >= 0
        if self.size > 1:
            fix_current_head = self.current is self.head
            for _ in range(n):
                self.append(self.pop_left())
                self.current = self.current.next
                self.current_idx += 1
            if fix_current_head:
                self.set_current_head()

    def to_list(self):
        current_idx = self.current_idx
        self.set_current_head()
        res = []
        for _ in range(self.size - 1):
            res.append(self.current.item)
            self.next()
        res.append(self.current.item)
        self.set_current_nth(current_idx)
        return res

    def get_priority_queue(self, key_hook=None):
        if key_hook is None:
            key_hook = lambda x: x
        pq = PriorityQueue()
        self.set_current_head()
        for _ in range(self.size - 1):
            pq.add((key_hook(self.current.item), self.current.item))
            self.next()
        pq.add((key_hook(self.current.item), self.current.item))
        return pq

    def get_sorted(self, key_hook=None, reverse=False):
        pq = self.get_priority_queue(key_hook)
        ll = LinkedList(circular=self.circular)
        if reverse:
            while not pq.is_empty():
                ll.append_left(pq.pop_min())
        else:
            while not pq.is_empty():
                ll.append(pq.pop_min())
        return ll


if __name__ == "__main__":
    ll = LinkedList(circular=True)
    ll.append(24)
    ll.append(4)
    ll.append(5)
    ll.append(64)
    ll.append(8)
    ll.insert_at_index(33, 1)
    ll.set_current_head()
    print(ll.current, ll.current.item, ll.current_idx, ll.to_list())
    ll.pop_left()
    print(ll.current, ll.current.item, ll.current_idx, ll.to_list())
    ll.append_left(24)
    print(ll.current, ll.current.item, ll.current_idx, ll.to_list())
    ll.set_current_tail()
    print(ll.current, ll.current.item, ll.current_idx, ll.to_list())
    ll.pop()
    print(ll.current, ll.current.item, ll.current_idx, ll.to_list())
    ll.append(9)
    print(ll.current, ll.current.item, ll.current_idx, ll.to_list())
    sorted_ll = ll.get_sorted(reverse=False)
    # print(ll, sorted_ll)
    print(ll.current, ll.current.item, ll.current_idx, ll.to_list())
    print(
        sorted_ll.current,
        sorted_ll.current.item,
        sorted_ll.current_idx,
        sorted_ll.to_list(),
    )
    ll.set_current_nth(3)
    sorted_ll.set_current_nth(1)
    print(ll.current, ll.current.item)
    print(sorted_ll.current, sorted_ll.current.item)
    print(id(ll.current.item) == id(sorted_ll.current.item))
