input = [list(i) for i in open("dia08_input.txt", "r").read().strip().split("\n")]

# Parte 1
visible_trees = 0
n_rows = len(input)
n_cols = len(input[0])
for i in range(1, n_rows - 1):
    for j in range(1, n_cols - 1):
        tree = input[i][j]
        max_upper_trees = max([input[k - 1][j] for k in range(i, 0, -1)])
        max_bottom_trees = max([input[k + 1][j] for k in range(i, n_rows - 1)])
        max_left_trees = max([input[i][k - 1] for k in range(j, 0, -1)])
        max_right_trees = max([input[i][k + 1] for k in range(j, n_cols - 1)])
        if (
            tree > max_upper_trees
            or tree > max_left_trees
            or tree > max_bottom_trees
            or tree > max_right_trees
        ):
            visible_trees += 1

print(visible_trees + 2 * n_cols + 2 * n_rows - 4)

# Parte 2
best_tree = (0, 0, 0)
best_scenic_score = 0
n_rows = len(input)
n_cols = len(input[0])
for i in range(1, n_rows - 1):
    for j in range(1, n_cols - 1):
        tree = int(input[i][j])
        upper_score = 0
        bottom_score = 0
        left_score = 0
        right_score = 0
        for k in range(i, 0, -1):
            another_tree = int(input[k - 1][j])
            upper_score += 1
            if tree <= another_tree:
                break
        for k in range(i, n_rows - 1):
            another_tree = int(input[k + 1][j])
            bottom_score += 1
            if tree <= another_tree:
                break
        for k in range(j, 0, -1):
            another_tree = int(input[i][k - 1])
            left_score += 1
            if tree <= another_tree:
                break
        for k in range(j, n_cols - 1):
            another_tree = int(input[i][k + 1])
            right_score += 1
            if tree <= another_tree:
                break
        scenic_score = upper_score * bottom_score * left_score * right_score
        if scenic_score > best_scenic_score:
            best_scenic_score = scenic_score
            best_tree = (tree, i, j)


print(best_scenic_score, best_tree)
