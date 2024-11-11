# For your imports look at the rulebook to see what is allowed
# Pour les import permis allez vous référer au livre des règlements
"""part_3.py
This is the template file for the part 3 of the Prelim 1.
Ceci est le fichier template pour la partie 3 du Prelim 1.
"""
import math

class Node():
    value:int
    left = None
    right = None

    def __init__(self, value:int):
        self.value = value

    def setLeft(self, left):
        self.left = left

    def setRight(self, right):
        self.right = right

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
    dik = dict(zip(values, letters))

    values.sort()

    root = make_tree(values)
    answ = BFS(root)


    return [dik[num] for num in answ]


def make_tree(values:list[int])->Node:
    #Find central index of list
    central_i = math.floor(len(values)/2)
    print(values)
    print("LE CENTRE:" + str(central_i) + " " + str(values[central_i]))
    
    #Create Node
    node = Node(values[central_i])

    if len(values)>1:
        node.setLeft(make_tree(values[0:central_i]))
        node.setRight(make_tree(values[central_i+1:len(values)]))

    print("Node OK! Value:" + str(node.value))
    return node

def BFS(root:Node)->list[int]:
    if root is None:
        return
    
    queue: list[Node]=[]
    queue.append(root)

    anws:list[int] = []

    while queue:
        node = queue.pop(0)  # take a node from the front of the queue
        anws.append(node.value)

        # enqueue left child
        if node.left:
            queue.append(node.left)
        # enqueue right child
        if node.right:
            queue.append(node.right)

    return anws


# def split_at_center(numbers: list[int]) -> tuple[list[int]|None, int, list[int]|None]:
#     """Find center and"""
#     if len(numbers) == 1:
#         return [None, numbers[0], None]
#     center_index = math.floor(len(numbers)/2)
#     l = numbers[0:center_index]
#     center = numbers[center_index]
#     r = numbers[center_index+1:-1]
#     return [l, center, r]


# def make_inverse_solution(answ: list[int], numbers: list[int]):
#     if len(numbers) == 0:
#         return
#     [l, center, r] = split_at_center(numbers)
#     if r is not None and l is not None:
#         make_inverse_solution(answ, r)
#         make_inverse_solution(answ, l)
#     answ.append(center)



