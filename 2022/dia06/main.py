input = open("input.txt", "r").read().strip()

# Parte 1
marker_length = 4
for i in range(len(input) - marker_length):
    if len(set(input[i : i + marker_length])) > (marker_length - 1):
        print(i + marker_length)
        break

# Parte 2
marker_length = 14
for i in range(len(input) - marker_length):
    if len(set(input[i : i + marker_length])) > (marker_length - 1):
        print(i + marker_length)
        break
