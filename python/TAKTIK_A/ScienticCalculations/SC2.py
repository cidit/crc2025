### Pi or almost (15 points) ###
import random
def solve() -> float:
    inCercle = 0
    outCercle = 0
    for _ in range(1000000):
        x = random.uniform(-1, 1)
        y = random.uniform(-1, 1)
        outCercle += 1
        if (x**2 + y**2 < 1):
            inCercle += 1

    return 4* (inCercle/outCercle)