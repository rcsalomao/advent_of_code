cals = [sum(list(map(int, elf.split()))) for elf in open("input.txt", "r").read().replace("\n\n", " ").split(" ")]

# Parte 1
print(max(cals))
# Parte 2
print(sum(sorted(cals, reverse=True)[:3]))
