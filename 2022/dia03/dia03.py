import string

# Parte 1
rucksacks_items = open("dia03_input.txt", "r").read().strip().split("\n")
rucksacks_items_count = [len(rucksacks_items[i]) for i in range(len(rucksacks_items))]
rucksacks_compartment1_items = [rucksacks_items[i][: int(rucksacks_items_count[i] / 2)] for i in range(len(rucksacks_items))]
rucksacks_compartment2_items = [rucksacks_items[i][int(rucksacks_items_count[i] / 2) :] for i in range(len(rucksacks_items))]
repeated_items = [
    set(rucksacks_compartment1_items[i]).intersection(set(rucksacks_compartment2_items[i])).pop()
    for i in range(len(rucksacks_items))
]
pontuacao = dict(
    [(string.ascii_lowercase[i], i + 1) for i in range(len(string.ascii_lowercase))]
    + [(string.ascii_uppercase[i], i + 27) for i in range(len(string.ascii_uppercase))]
)
pontuacao_total = sum([pontuacao[item] for item in repeated_items])
print(pontuacao_total)

# Parte 2
elfs_groups = [
    (rucksacks_items[i * 3], rucksacks_items[i * 3 + 1], rucksacks_items[i * 3 + 2])
    for i in range(int(len(rucksacks_items) / 3))
]
badge_groups = [
    set(rucksack1).intersection(set(rucksack2)).intersection(set(rucksack3)).pop()
    for rucksack1, rucksack2, rucksack3 in elfs_groups
]
pontuacao_total_badge = sum([pontuacao[item] for item in badge_groups])
print(pontuacao_total_badge)
