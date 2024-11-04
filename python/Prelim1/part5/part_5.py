# For your imports look at the rulebook to see what is allowed
# Pour les import permis allez vous référer au livre des règlements
"""part_5.py
This is the template file for the part 5 of the Prelim 1.
Ceci est le fichier template pour la partie 5 du Prelim 1.
"""

# helper function to visualize your tree
def print_tree(tree_strings):
    for line in tree_strings:
        print(line)
    print()

def part_5(message):
    """
    Decrypts the message from a tring to a visualy represented tree

    Parameters:
        message str: The secret message to decrypt

    Returns:
        [str]: The strings that represents the layers of the tree to display
    """
    ### YOU CAN USE THAT EXAMPLE TO YOUR ADVANTAGE ###
    tree_strings = ["     1      ",
                    "    ↙ ↘     ",
                    "   ↙   ↘   ",
                    "  2     3  ",
                    " ↙ ↘   ↙ ↘ ",
                    "4   5 6   7"]
    
    ### You code goes here ###
    ### Votre code va ici ###
    print_tree(tree_strings)




    return tree_strings
