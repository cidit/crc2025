# For your imports look at the rulebook to see what is allowed
# Pour les import permis allez vous référer au livre des règlements
"""part_3.py
This is the template file for the part 3 of the Prelim 2.
Ceci est le fichier template pour la partie 3 du Prelim 2.
"""

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
    print(lev_values)


    return CAPTCHA

# Build the levenstein array for two words
def build_lev_array(words:list[str]):
    lev_table:list[list[int]] = (len(words[0])+1)*[(len(words[1])+1) * [None]]

    for i in range(len(words[0])+1): #Rows
        for j in range(len(words[1])+1): #Columns
            lev_table[i][j] = lev(words[0][j:i], words[1][i:j])
    
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
