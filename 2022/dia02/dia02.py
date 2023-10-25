# input = """
# A Y
# B X
# C Z
# """
input = open("dia02_input.txt", "r").read()

oponent_map = {"A": "rock", "B": "paper", "C": "scissor"}
me_map = {
    "rock": {"X": ("scissor", 0), "Y": ("rock", 3), "Z": ("paper", 6)},
    "paper": {"X": ("rock", 0), "Y": ("paper", 3), "Z": ("scissor", 6)},
    "scissor": {"X": ("paper", 0), "Y": ("scissor", 3), "Z": ("rock", 6)},
}
shape_map = {"rock": 1, "paper": 2, "scissor": 3}


def game_rules(game):
    oponent = oponent_map[game[0]]
    me, partial_score = me_map[oponent][game[1]]
    return shape_map[me] + partial_score


print(
    sum(
        [
            game_rules(game)
            for game in [game.split(" ") for game in input.strip().split("\n")]
        ]
    )
)
