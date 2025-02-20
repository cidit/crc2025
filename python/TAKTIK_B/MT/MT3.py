# We go to hyperspace! (90 points) #
import math
def solve(vaisseau: tuple[int, int], destination: tuple[int, int], astres: tuple[tuple[int, int], int]) -> list[tuple[int, int]]:
    a = (destination[1] - vaisseau[1])/(destination[0] - vaisseau[0])
    b = vaisseau[1] - a*vaisseau[0]

    isColliding = True
    while isColliding:
        points = []
        for planet in astres:
            points.append(find_intersections(planet[0][0], planet[0][1], planet[1], a, b))

        if not points:
            isColliding = False
        print(points)
        





def find_intersections(h, k, r, m, c):

    A = 1 + m**2
    B = 2 * (m * (c - k) - h)
    C = h**2 + (c - k)**2 - r**2

    discriminant = B**2 - 4 * A * C

    if discriminant < 0:
        return []
    elif discriminant == 0:
        x = -B / (2 * A)
        y = m * x + c
        return [(x, y)]
    else:
        sqrt_discriminant = math.sqrt(discriminant)
        x1 = (-B + sqrt_discriminant) / (2 * A)
        x2 = (-B - sqrt_discriminant) / (2 * A)
        y1 = m * x1 + c
        y2 = m * x2 + c
        return [(x1, y1), (x2, y2)]