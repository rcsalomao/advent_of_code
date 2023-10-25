import re
import numpy as np


input = """
Sensor at x=2, y=18: closest beacon is at x=-2, y=15
Sensor at x=9, y=16: closest beacon is at x=10, y=16
Sensor at x=13, y=2: closest beacon is at x=15, y=3
Sensor at x=12, y=14: closest beacon is at x=10, y=16
Sensor at x=10, y=20: closest beacon is at x=10, y=16
Sensor at x=14, y=17: closest beacon is at x=10, y=16
Sensor at x=8, y=7: closest beacon is at x=2, y=10
Sensor at x=2, y=0: closest beacon is at x=2, y=10
Sensor at x=0, y=11: closest beacon is at x=2, y=10
Sensor at x=20, y=14: closest beacon is at x=25, y=17
Sensor at x=17, y=20: closest beacon is at x=21, y=22
Sensor at x=16, y=7: closest beacon is at x=15, y=3
Sensor at x=14, y=3: closest beacon is at x=15, y=3
Sensor at x=20, y=1: closest beacon is at x=15, y=3
"""
input = input.strip().split("\n")
input = open("dia15_input.txt", "r").read().strip().split("\n")

sensor_data = []
for line in input:
    f = re.findall(r"(-?\d+)", line)
    sensor_data.append(((int(f[0]), int(f[1])), (int(f[2]), int(f[3]))))

min_x = sensor_data[0][0][0]
max_x = sensor_data[0][0][0]
for data in sensor_data:
    sensor = data[0]
    beacon = data[1]
    x_sensor, y_sensor = sensor
    x_beacon, y_beacon = beacon
    min_x = min(min_x, x_sensor, x_beacon)
    max_x = max(max_x, x_sensor, x_beacon)


def calc_isnt_positions(delta_x, scan_line, y_scan_line, sensor_data, part1=False):
    scan_line[:] = 0
    for data in sensor_data:
        sensor = data[0]
        beacon = data[1]
        x_sensor, y_sensor = sensor
        x_beacon, y_beacon = beacon
        dx = abs(x_sensor - x_beacon)
        dy = abs(y_sensor - y_beacon)
        w_losangulo = dx + dy
        dssl = abs(y_sensor - y_scan_line)
        dw = w_losangulo - dssl
        # print(dx, dy, dssl, w_losangulo, dw)
        scan_line[
            min(max(x_sensor - min_x - max(0, dw), 0), delta_x) : min(
                max(x_sensor - min_x + (1 if dw >= 0 else 0) + max(0, dw), 0), delta_x
            )
        ] = 1
        if part1:
            if y_scan_line == y_beacon:
                scan_line[x_beacon - min_x] = 0
    return scan_line


def run_parte1():
    global min_x
    global max_x
    min_x -= 500_000
    max_x += 500_000
    delta_x = max_x - min_x + 1
    scan_line = np.zeros(delta_x, dtype=int)
    scan = calc_isnt_positions(delta_x, scan_line, 2_000_000, sensor_data, part1=True)
    print(scan.sum())


def run_parte2():  # Ok. Tenho plena convicção que essa solução funciona. MAS, ela é muito lenta (seria necessária outra solução mais performática.)
    global min_x
    global max_x
    min_x = 0
    max_x = 4_000_000
    # max_x = 20
    delta_x = max_x - min_x + 1
    scan_line = np.zeros(delta_x, dtype=int)
    y_beacon = 0
    x_beacon = 0
    for i in range(4_000_000):
        scan = calc_isnt_positions(delta_x, scan_line, i, sensor_data)
        if not i % 10:
            print(i)
        if scan.sum() < delta_x:
            y_beacon = i
            x_beacon = np.where(scan == 0)[0][0]
            break
        elif i > 4_000_000:
            print("não deu certo :c")
            break
        # print(i, sum(scan), x_beacon, y_beacon, x_beacon * 4_000_000 + y_beacon)
    print(x_beacon, y_beacon, x_beacon * 4_000_000 + y_beacon)


run_parte1()
# run_parte2()
