# For your imports look at the rulebook to see what is allowed
# Pour les import permis allez vous référer au livre des règlements
"""part_5.py
This is the template file for the part 5 of the Prelim 2.
Ceci est le fichier template pour la partie 5 du Prelim 2.
"""
from math import cos, tan, sqrt, pi

GRAVITY = 9.81

def part_5(h: int, d: int, angle: int):
    """
    Computes the initial speed of a projectile considering a target for a catapult
    
    Parameters:
        h int: the height from which the caapult is shooting
        d int: the distance from the castloe walls to the target
        angle int: the angle of launch in degrees from the horizon

    Returns:
        float: the speed at which the projectile needs to be thrown
    """
    initial_speed = 0
    angle = rad(angle)
    
    initial_speed = sqrt(
        (GRAVITY * d**2)/
        (2 * cos(angle)**2 * (h + d * tan(angle)))
    )
    initial_speed = round(initial_speed, 2)

    return initial_speed

def rad(deg: int) -> float:
    return deg * pi/ 180.0