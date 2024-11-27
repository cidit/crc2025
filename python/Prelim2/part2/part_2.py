# For your imports look at the rulebook to see what is allowed
# Pour les import permis allez vous référer au livre des règlements
"""part_2.py
This is the template file for the part 2 of the Prelim 2.
Ceci est le fichier template pour la partie 2 du Prelim 2.
"""
import math

def part_2(d: int, h: int, b: int, m_cost: float, margin: float) -> float:
    """
    Computes the cost of custom made top hats
    
    Parameters:
        d int: the width of the opening of the hat
        h int: the height of the hat
        b int: the width of the brim
        m_cost float: the cost of the material
        margin float: the profit margin you want to make

    Returns:
        float: the price of the custom hat
    """
    _price = 0
    
    _top = top(d)
    _side = side(d, h)
    _brim = brim(_top, d, b)
    _price = price(_top+_side+_brim, m_cost, margin)
    return _price

def top(d:int)->float:
    return math.pi * pow(d/2.0, 2)

def side(d:int, h:int)->float:
    return 2 * math.pi * (d/2.0) * h

def brim(top:float, d:int, b:int)->float:
    return math.pi * pow(b + d/2.0, 2) - top

def price(materials:float, cost:float, profit:float):
    return round(materials * cost * profit * 100)/100