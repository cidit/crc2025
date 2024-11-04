# For your imports look at the rulebook to see what is allowed
# Pour les import permis allez vous référer au livre des règlements
"""part_1.py
This is the template file for the part 1 of the Prelim 1.
Ceci est le fichier template pour la partie 1 du Prelim 1.
"""

def part_1(sentences: str):
    """
    Take the lowest letter for each sentence in a string

    Parameters:
        sentences str: A string containing the sentences to analyse

    Returns:
        [str]: A list a the lowest letter in each sentence
    """
    lowest_letters = []

    for i in len(sentences):
        current_lowest:str = None

        # Append letter si on a fini la phrase
        if sentences[i] in ['.', '!', '?'] and not current_lowest==None:
            lowest_letters.append(current_lowest)
            current_lowest = None
            continue #Skip reste de la loop
        
        # Change Letter if none or new lowest
        if current_lowest==None or sentences[i] < current_lowest:
            current_lowest = sentences[i]    

    return lowest_letters

