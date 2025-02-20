### Cylinder ratio (10 points) ###
import math
def solve(d: int, h: int):
    aire = 2*(math.pi * (d/2)**2) + 2*math.pi*(d/2)*h
    volume = math.pi * (d/2)**2 * h

    return round(aire/volume, 2)