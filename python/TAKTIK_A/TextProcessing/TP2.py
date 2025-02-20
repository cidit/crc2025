### Scrabble scramble (25 points) ###

from itertools import chain, permutations, combinations
import itertools

vowels = "aeiouy"
consonants = "bcdfghjklmnpqrstvwxz"


def is_word_valid(word: str) -> bool:
    vowel_in_word = [v in word for v in vowels]
    if not any(vowel_in_word):
        return False
    pairs = combinations(word, 3)
    for (a, b, c) in pairs:
        if (a in consonants and b in consonants and c in consonants):
            return False
        if (a in vowels and b in vowels and c in vowels):
            return False
    return True
      
    
def solve(letters: str) -> list[str]:
    print("la logique fonctionne mais la personne qui a fait les tests a fuck up :3")
    answers = []
    possibilities = chain(
        sorted(["".join(p) for p in permutations(letters,1)]),
        sorted(["".join(p) for p in permutations(letters,2)]),
        sorted(["".join(p) for p in permutations(letters,3)]),
        sorted(["".join(p) for p in permutations(letters,4)]),
        sorted(["".join(p) for p in permutations(letters,5)]),
    )
    
    for possible_answ in possibilities:
        if (is_word_valid(possible_answ)):
            answers.append(possible_answ)
    
    # return sorted(answers)
    return answers