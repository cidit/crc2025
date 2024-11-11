# For your imports look at the rulebook to see what is allowed
# Pour les import permis allez vous référer au livre des règlements
"""part_5.py
This is the template file for the part 5 of the Prelim 1.
Ceci est le fichier template pour la partie 5 du Prelim 1.
"""
import unicodedata
import math

# Classes #
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

# helper function to visualize your tree
def print_tree(tree_strings):
    for line in tree_strings:
        print(line)
    print()

# Solution #
def part_5(message):
    """
    Decrypts the message from a tring to a visualy represented tree

    Parameters:
        message str: The secret message to decrypt

    Returns:
        [str]: The strings that represents the layers of the tree to display
    """
    ### YOU CAN USE THAT EXAMPLE TO YOUR ADVANTAGE ###
    tree_strings = ["     1     ",
                    "    ↙ ↘    ",
                    "   ↙   ↘   ",
                    "  2     3  ",
                    " ↙ ↘   ↙ ↘ ",
                    "4   5 6   7"]
    
    ### You code goes here ###
    ### Votre code va ici ###
    message = strip_accents(message)
    sentences = split_sentences(message)

    lowest_letters = list(map(find_lowest, sentences))
    multiplications = [find_mult(s) for s in sentences]

    # Match mult to letters
    dik = dict(zip(multiplications, lowest_letters))

    multiplications.sort()

    root = make_tree(multiplications)
    answ = BFS(root)

    letters = [dik[num] for num in answ]

    for i in range(len(letters)):
        for j in range(len(tree_strings)):
            tree_strings[j] = tree_strings[j].replace(str(i+1), letters[i])
            
    print_tree(tree_strings)

    return tree_strings

# Part 1 and 2 #
def split_sentences(sentences: str) -> list[str]:
    """Split in invidual sentence"""

    replaced = "".join([c if c not in ".!?" else "\n" for c in sentences])
    return replaced.splitlines()


def find_lowest(sentence: str) -> str | None:
    """Return the lowest letter"""

    valid = [chara for chara in sentence if chara >= 'A' and chara <= 'z'] 
    
    if len(valid)==0:
        return None
    
    valid.sort()
    return valid[0]

def find_mult(sentence: str) -> int | None:
    """Return multiplication of vowels xconsonnant"""

    valid = [chara for chara in sentence if chara >= 'A' and chara <= 'z'] 
    
    if len(valid)==0:
        return None

    chars_in_group = lambda group: [char for char in sentence if char.upper() in group]

    vowels = chars_in_group("AEIOUY")
    consonnants = chars_in_group("QWRTPSDFGHJKLZXCVBNM")

    return len(vowels)*len(consonnants)

def strip_accents(s):
    # https://stackoverflow.com/questions/517923/what-is-the-best-way-to-remove-accents-normalize-in-a-python-unicode-string
    return ''.join(c for c in unicodedata.normalize('NFD', s) if unicodedata.category(c) != 'Mn')

# Part 3 #
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