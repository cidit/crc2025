### Paint is really expensive! (30 points) ###

def solve(H: float, L: float, b: float, d: float) -> float:
    base = H * L
    #print("base ", base)
    courbe = 0.0
    div = L/d
    #print("div ", div)

    for i in range(d):
        x = (i+1)*div
        a = div * b**x
        #print("aire ", a, " i ", i+1)
        courbe += a

    return round(base+courbe, 3)