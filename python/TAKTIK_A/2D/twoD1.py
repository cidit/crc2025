### Euclid and Manhatthan (15 points) ###
import math
def solve(grid: list[str]) -> float:

    xa = 0
    ya = 0
    xb = 0
    xy = 0
    #Find letters
    positions:list[tuple[int, int]]
    for i in range(len(grid)):
        try:
            xa = grid[i].index("a")
            ya = i
            continue
        except :
            pass
        try:
            xb = grid[i].index("b")
            yb = i
        except :
            pass
        

    #Find distances
    #Euclid
    eucl = math.sqrt(abs(xb-xa)**2 + abs(yb-ya)**2)
    mat = (abs(xb-xa) + abs(yb-ya))

    return round(mat + eucl, 2)
