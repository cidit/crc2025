# For your imports look at the rulebook to see what is allowed
# Pour les import permis allez vous référer au livre des règlements
"""part_4.py
This is the template file for the part 4 of the Prelim 1.
Ceci est le fichier template pour la partie 4 du Prelim 1.
"""

# helper function to visualize your tree
def print_tree(tree_strings):
    for line in tree_strings:
        print(line)
    print()

def part_4(letters):
    """
    Formats the values of a tree to put in a array of strings

    Parameters:
        letters [str]: The 7 letters to display in the visual of the tree ordered by layers

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
    

    for i in range(len(letters)):
        for j in range(len(tree_strings)):
            tree_strings[j] = tree_strings[j].replace(str(i+1), letters[i])


    print_tree(tree_strings)




    return tree_strings
