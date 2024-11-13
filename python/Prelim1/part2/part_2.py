# For your imports look at the rulebook to see what is allowed
# Pour les import permis allez vous référer au livre des règlements
"""part_2.py
This is the template file for the part 2 of the Prelim 1.
Ceci est le fichier template pour la partie 2 du Prelim 1.
"""
import unicodedata

def part_2(sentences: str):
    """
    This function find the number of vowel and consonnants and multiply them together for every sentence in a string

    Parameters:
        sentence str: The sentences that are analysed

    Returns:
        exemple:
        [int]: The multiplication of vowels and consonnant
    """
    multiplications = []
    
    sentences = strip_accents(sentences)
    sentences = split_sentences(sentences)

    multiplications = [find_mult(s) for s in sentences]

    return multiplications


def split_sentences(sentences: str) -> list[str]:
    """Split in invidual sentence"""

    replaced = "".join([c if c not in ".!?" else "\n" for c in sentences])
    return replaced.splitlines()


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