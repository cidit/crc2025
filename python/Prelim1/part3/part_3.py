# For your imports look at the rulebook to see what is allowed
# Pour les import permis allez vous référer au livre des règlements
"""part_3.py
This is the template file for the part 3 of the Prelim 1.
Ceci est le fichier template pour la partie 3 du Prelim 1.
"""
import math

def part_3(values, letters):
    """
    Arrange the received values in a tree and return the order of the corresponding letters

    Parameters:
        values [int]: Array of the 7 values to decide the order in the tree
        letters [str]: Array of the 7 letters to display in the tree

    Returns:
        [str]: The letters ordered from the tree given by the values
    """

    ### You code goes here ###
    ### Votre code va ici ### HAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA!

    tree_letters = []
    dick = dict(zip(values, letters))

    values.sort()

    
    answ = []
    make_inverse_solution(answ, values)
    answ.reverse()

    return [dick[num] for num in answ]


def split_at_center(numbers: list[int]) -> tuple[list[int]|None, int, list[int]|None]:
    """Find center and"""
    if len(numbers) == 1:
        return [None, numbers[0], None]
    center_index = math.floor(len(numbers)/2)
    l = numbers[0:center_index]
    center = numbers[center_index]
    r = numbers[center_index+1:-1]
    return [l, center, r]


def make_inverse_solution(answ: list[int], numbers: list[int]):
    if len(numbers) == 0:
        return
    [l, center, r] = split_at_center(numbers)
    if r is not None and l is not None:
        make_inverse_solution(answ, r)
        make_inverse_solution(answ, l)
    answ.append(center)

class Node:
    a: list[int]
    b: list[int]
    c: int

