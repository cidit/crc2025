# For your imports look at the rulebook to see what is allowed
# Pour les import permis allez vous référer au livre des règlements
"""part_3.py
This is the template file for the part 3 of the Prelim 2.
Ceci est le fichier template pour la partie 3 du Prelim 2.
"""

from dataclasses import dataclass

def part_3(words):
    """
    Generate a CAPTCHA from the Levenstein distance of 2 words

    Parameters:
        words [string]: the 2 words from which you will need to find the distance

    Returns:
        [str]: The array of strings composing the CAPTCHA
    """
    CAPTCHA = []

    ### You code goes here ###
    ### Votre code va ici ###

    lev_values = build_lev_array(words)
    CAPTCHA = generate_levi_instructions(lev_values, CAPTCHA, words)
    #generate_capCHAT(len(levi_instructions)) # len(levi_instructions) is the levenstein distance
    print(CAPTCHA)
    return CAPTCHA

# Use the algo to generate the capchat
def generate_capCHAT():
    pass

# ['i', i, j]
# ['d', i, j]
# ['s', i, j]
    
def generate_levi_instructions(lev_array:list[list[int]], capchat, words:list[str]):
    i = len(lev_array)-1 #Row
    j = len(lev_array[0])-1 #Column
    

    # [5] insertion
    # [5] deletion
    # [5] subs
    
    eye_options = ["|'_'|", "|-_-|", "|*_*|", "|@_@|"]
    
    capchat = [list(" ^-^ "),
               list(eye_options[lev_array[i][j]%4]),
               list(" > < ")]
    
    # ^-^  #  ^-^  #  ^-^  #  ^-^
    #|'_'| # |-_-| # |*_*| # |@_@|
    # > <  #  > <  #  > <  #  > <
    
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
                capchat[2][j%5] = words[1][j]
            else:
                if (horizontal <= vertical and 
                    horizontal <= current):
                    #Insertion
                    j-=1
                    # capchat[0][j%5] = words[1][j]
                    capchat[0].insert(j%5, words[1][j])
                else:
                    if (vertical <= horizontal and 
                        vertical <= current):
                        #Delete
                        i=i-1
                        # capchat[1][i%5] = ' '
                        del capchat[1][i%5]
                    else:
                        continue # ici, on fais vraiment riens.
        else:
            if (horizontal <= vertical and 
                horizontal <= current):
                #Insertion
                j-=1
                # capchat[0][j%5] = words[1][j]
                capchat[0].insert(j%5, words[1][j])
            else:
                #Deletion
                i -= 1
                # capchat[1][i%5] = ' '
                del capchat[1][i%5]



    return list(map(lambda l: "".join(l), capchat))
               
               
# Build the levenstein array for two words
def build_lev_array(words:list[str])->list[list[int]]:
    # Create an array of the necessary size
    lev_table:list[list[int]] = [[0 for _ in range(len(words[1])+1)] for _ in range(len(words[0])+1)]

    # Fill the array
    for i in range(len(words[0])+1): #Rows
        for j in range(len(words[1])+1): #Columns
            lev_table[i][j] = lev(words[0][0:i], words[1][0:j])
            #print(words[0][0:i] + "," + words[1][0:j] + "," + str(lev_table[i][j]))
        #print()
    
    return lev_table


# Find de distance of Levenshtein between a and b
def lev(a, b):
    #Find lenght of words
    _a = len(a)
    _b = len(b)

    if min(_a, _b)==0:
        return max(_a, _b)
    elif a[0] == b[0]:
        return lev(a[1:], b[1:])
    else:
        return 1+min(lev(a[1:], b), lev(a, b[1:]), lev(a[1:], b[1:]))


def print_lev_matrix(matrix: list[list[int]]):
    for row in matrix:
        print(row)

# TEST 1
#     s i t t i n g
#   0 1 2 3 4 5 6 7 
# k 1 1 2 3 4 5 6 7
# i 2 2 1 2 3 4 5 6
# t 3 3 2 1 2 3 4 5
# t 4 4 3 2 1 2 3 4
# e 5 5 4 3 2 2 3 3
# n 6 6 5 4 3 3 2 3
#