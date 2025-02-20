### Averaging in circles (25 points) ###

def solve(angle: list[int]):
    for i in range(len(angle)):
        while angle[i] > 360:
            angle[i] = angle[i]-360

        while angle[i] < -360:
            angle[i] = angle[i]+360

        if angle[i] > 180:
            angle[i] = angle[i]-360

    n = len(angle)
    moy = sum(angle)/n
    if moy < 0:
        moy += 360

    return round(moy, 2)