import re
from copy import deepcopy
from functools import reduce
from operator import mul
from PIL import Image, ImageFont, ImageDraw

# input = open("./mock_input.txt", "r").read().strip().splitlines()
input = open("./input.txt", "r").read().strip().splitlines()
# print(input)


robots_positions: list[tuple[int, int]] = []
robots_velocities: list[tuple[int, int]] = []
for line in input:
    le, r = line.split(" ")
    x, y = re.match(r"p=(\d+),(\d+)", le).groups()  # type: ignore
    vx, vy = re.match(r"v=(-?\d+),(-?\d+)", r).groups()  # type: ignore
    robots_positions.append((int(x), int(y)))
    robots_velocities.append((int(vx), int(vy)))
initial_robots_positions = deepcopy(robots_positions)


def print_grid(
    grid_size: tuple[int, int], robots_positions: list[tuple[int, int]]
) -> None:
    max_x, max_y = grid_size
    for i in range(max_y):
        for j in range(max_x):
            num_robots = robots_positions.count((j, i))
            if num_robots == 0:
                char = "."
            else:
                char = str(num_robots)
            print(char, end="")
        print()


def gen_text_output(
    grid_size: tuple[int, int], robots_positions: list[tuple[int, int]]
):
    max_x, max_y = grid_size
    output = ""
    for i in range(max_y):
        for j in range(max_x):
            num_robots = robots_positions.count((j, i))
            if num_robots == 0:
                char = "."
            else:
                # char = str(num_robots)
                char = "#"
            output += char
        output += "\n"
    return output


def update_robots_positions(
    grid_size: tuple[int, int],
    robots_positions: list[tuple[int, int]],
    iteration_number: int,
):
    gsx, gsy = grid_size
    for r in range(len(robots_positions)):
        x0, y0 = initial_robots_positions[r]
        vx, vy = robots_velocities[r]
        robots_positions[r] = (
            (x0 + iteration_number * vx) % gsx,
            (y0 + iteration_number * vy) % gsy,
        )


def sum_robots_per_quadrant(grid_size, robots_positions):
    max_x, max_y = grid_size
    dgsx = max_x // 2
    dgsy = max_y // 2
    num_robots = {
        "1o": 0,
        "2o": 0,
        "3o": 0,
        "4o": 0,
    }
    for pos in robots_positions:
        x, y = pos
        if 0 <= x < dgsx:
            if 0 <= y < dgsy:
                num_robots["1o"] += 1
            elif (max_y - dgsy) <= y < max_y:
                num_robots["3o"] += 1
            else:
                ...
        elif (max_x - dgsx) <= x < max_x:
            if 0 <= y < dgsy:
                num_robots["2o"] += 1
            elif (max_y - dgsy) <= y < max_y:
                num_robots["4o"] += 1
            else:
                ...
        else:
            ...
    return num_robots


def parte_1() -> None:
    # grid_size = (11, 7)
    grid_size = (101, 103)
    # print_grid(grid_size, robots_positions)
    iteration_number = 100
    update_robots_positions(grid_size, robots_positions, iteration_number)
    # print()
    # print_grid(grid_size, robots_positions)
    num_robots_per_quadrant = sum_robots_per_quadrant(grid_size, robots_positions)
    safety_factor = reduce(mul, num_robots_per_quadrant.values(), 1)
    print(safety_factor)


def parte_2() -> None:
    # grid_size = (11, 7)
    grid_size = (101, 103)
    # print_grid(grid_size, robots_positions)
    max_iter = 10000
    for iteration_number in range(0, max_iter):
        print(f"Iteração {iteration_number} de {max_iter}")
        update_robots_positions(grid_size, robots_positions, iteration_number)
        # print()
        flag = True
        for robot in robots_positions:
            if robots_positions.count(robot) > 1:
                flag = False
        if flag:
            img = Image.new("RGB", (320, 830), color=(255, 255, 255))
            fnt = ImageFont.truetype("Pillow/Tests/fonts/FreeMono.ttf", 5)
            ImageDraw.Draw(img).text(
                (0, 0),
                gen_text_output(grid_size, robots_positions),
                font=fnt,
                fill=(0, 0, 0),
            )
            img.save(f"./imgs/img_{iteration_number:05}.png")


if __name__ == "__main__":
    parte_1()
    # parte_2()
