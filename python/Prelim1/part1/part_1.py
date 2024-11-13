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

    sentences = split_sentences(sentences)
    lowest_letters = list(map(find_lowest, sentences))

    return lowest_letters


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