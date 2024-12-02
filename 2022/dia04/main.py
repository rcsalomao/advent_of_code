section_assigment_input = open("input.txt", "r").read().strip()
elf_pairs = [pair.split(",") for pair in section_assigment_input.split("\n")]
elf_pairs = [
    [
        tuple(int(a) for a in elf_pairs[i][0].split("-")),
        tuple(int(a) for a in elf_pairs[i][1].split("-")),
    ]
    for i in range(len(elf_pairs))
]
total_number_elf_pairs = len(elf_pairs)

# Parte 1
total_overlap_count = 0
for elf_pair in elf_pairs:
    elf1_min, elf1_max = elf_pair[0]
    elf2_min, elf2_max = elf_pair[1]
    if (elf1_min <= elf2_min) and (elf1_max >= elf2_max):
        total_overlap_count += 1
    elif (elf1_min >= elf2_min) and (elf1_max <= elf2_max):
        total_overlap_count += 1
print(total_overlap_count)

# Parte 2
total_nonoverlap_count = 0
for elf_pair in elf_pairs:
    elf1_min, elf1_max = elf_pair[0]
    elf2_min, elf2_max = elf_pair[1]
    if (elf1_max < elf2_min) or (elf1_min > elf2_max):
        total_nonoverlap_count += 1
partial_overlap_count = total_number_elf_pairs - total_nonoverlap_count
print(partial_overlap_count)

