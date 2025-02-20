# Choose your path (30 points) #
import math


def group_choices(choices):
    

def solve(passengers: list[int]) -> list[str]:
    nbCol = round(math.log2(len(passengers)))
    nbChoice = nbCol - 1
    print(nbCol)

    col = []

    for i in range(nbCol):
        col.append(passengers[2**i-1:2**i+2**i-1])



        
    print(col)
    return