# For your imports look at the rulebook to see what is allowed
# Pour les import permis allez vous référer au livre des règlements
"""part_3.py
This is the template file for the part 3 of the Prelim 2.
Ceci est le fichier template pour la partie 3 du Prelim 2.
"""

from functools import cache

def part_3(words):
    """
    Generate a CAPTCHA from the Levenstein distance of 2 words

    Parameters:
        words [string]: the 2 words from which you will need to find the distance

    Returns:
        [str]: The array of strings composing the CAPTCHA
    """
    CAPTCHA = []
    
    lev_values = build_levenshtein_array(words)
    levi_instructions = generate_levenshtein_instructions(lev_values)
    levi_instructions.reverse()
    base = generate_base_capCHAT(lev_values[-1][-1])

    CAPTCHA = update_capCHAT(base, levi_instructions, words)
    print(CAPTCHA)
    return CAPTCHA


def generate_base_capCHAT(levenshtein_dist: int):
    '''Chooses the basic CapCHAT'''
    
    #  ^-^  #  ^-^  #  ^-^  #  ^-^  #
    # |'_'| # |-_-| # |*_*| # |@_@| #
    #  > <  #  > <  #  > <  #  > <  #
    
    eye_options = ["|'_'|", "|-_-|", "|*_*|", "|@_@|"]
    
    capchat = [list(" ^-^ "),
               list(eye_options[levenshtein_dist%4]),
               list(" > < ")]
    return capchat


def update_capCHAT(capCHAT, operations, words):
    '''Update the CapCHAT using the operations'''

    for [op, i, j] in operations:
        match op:
            case "i":
                capCHAT[0].insert(j%5, words[1][j])
            case "r":
                capCHAT[2][j%5] = words[1][j]
            case "d":
                del capCHAT[1][i%5]
            case "n":
                continue
    return list(map(lambda l: "".join(l), capCHAT))
    
    
def generate_levenshtein_instructions(lev_array: list[list[int]]):
    '''Creates a list of instructions using the backtracking algorithm'''

    # shape of instructions
    # insertion:   ['i', i, j]
    # deletion:    ['d', i, j]
    # replacement: ['r', i, j]
    i = len(lev_array)-1 #Row
    j = len(lev_array[0])-1 #Column
    
    ops = []
    
    while i > 0 and j > 0:
        current = lev_array[i][j]
        diagonnal = lev_array[i-1][j-1]
        vertical = lev_array[i-1][j]
        horizontal = lev_array[i][j-1]
        if (diagonnal <= vertical and 
            diagonnal <= horizontal and
            diagonnal <= current):
            i-=1
            j-=1
            if diagonnal == current-1:
                #Replacement
                ops.append(['r', i, j])
            else:
                if (horizontal <= vertical and 
                    horizontal <= current):
                    #Insertion
                    j-=1
                    ops.append(['i', i, j])
                else:
                    if (vertical <= horizontal and 
                        vertical <= current):
                        #Delete
                        i=i-1
                        ops.append(['d', i, j])
                    else:
                        ops.append(['n', i, j])
                        continue # ici, on fais vraiment riens.
        else:
            if (horizontal <= vertical and 
                horizontal <= current):
                #Insertion
                j-=1
                ops.append(['i', i, j])
            else:
                #Deletion
                i -= 1
                ops.append(['d', i, j])
    return ops
               
               

def build_levenshtein_array(words:list[str]) -> list[list[int]]:
    '''Generates the array containing the Levenshtein distances'''

    # Create an array of the necessary size
    lev_table = [[0 for _ in range(len(words[1])+1)] for _ in range(len(words[0])+1)]

    # Fill the array
    for i in range(len(words[0])+1): #Rows
        for j in range(len(words[1])+1): #Columns
            lev_table[i][j] = lev(words[0][0:i], words[1][0:j])
    
    return lev_table


@cache
def lev(a: str, b: str):
    '''Find the Levenshtein distance between word a and word b
    The cache attribut is used to speed up the process'''
    #Find lenght of words
    _a = len(a)
    _b = len(b)

    #Provided Algorithm
    if min(_a, _b)==0:
        return max(_a, _b)
    elif a[0] == b[0]:
        return lev(a[1:], b[1:])
    else:
        return 1+min(lev(a[1:], b), lev(a, b[1:]), lev(a[1:], b[1:]))


def print_lev_matrix(matrix: list[list[int]]):
    '''Pretty print the Levenshtein matrix'''
    for row in matrix:
        print(row)
