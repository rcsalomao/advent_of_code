import re

# input = open("./mock_input.txt", "r").read()
input = open("./input.txt", "r").read()
# print(input)

rules = [tuple(int(i) for i in s.split("|")) for s in re.findall(r".+\|.+", input)]
updates = [
    [int(page) for page in pages.split(",")] for pages in re.findall(r".+,.+", input)
]
rule_sets: dict[int, set[int]] = {}
for rule in rules:
    page_befor, page_after = rule
    if rule_sets.get(page_after):
        rule_sets[page_after].add(page_befor)
    else:
        rule_sets[page_after] = set([page_befor])
# print(rules)
# print(updates)
# print(rule_set)


def check_update_order(update, rule_sets):
    for i, u in enumerate(update):
        # print(i, u, update[i + 1 :])
        for r in update[i + 1 :]:
            try:
                if r in rule_sets[u]:
                    # print("deu ruim")
                    return (u, r, False)
            except KeyError:
                continue
    # print("deu boa")
    return (None, None, True)


def parte_1():
    valid_updates = []
    for update in updates:
        u, r, check = check_update_order(update, rule_sets)
        if check:
            valid_updates.append(update)
    print(sum([u[int(len(u) / 2)] for u in valid_updates]))


def parte_2():
    invalid_updates = []
    for update in updates:
        u, r, check = check_update_order(update, rule_sets)
        if not check:
            invalid_updates.append(update)
    # print(invalid_updates)
    n_invalid_updates = len(invalid_updates)
    max_iter = 1000
    flag = True
    iter = 0
    while flag and iter < max_iter:
        flag = False
        iter += 1
        for i in range(n_invalid_updates):
            update = invalid_updates[i]
            u, r, check = check_update_order(update, rule_sets)
            if check:
                continue
            flag = True
            j, k = update.index(u), update.index(r)
            update[k], update[j] = update[j], update[k]
    # print(invalid_updates)
    print(sum([u[int(len(u) / 2)] for u in invalid_updates]))

    pass


if __name__ == "__main__":
    parte_1()
    parte_2()
