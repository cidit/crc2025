### This is the test file and you are NOT supposed to modify it (unless you want to add more tests) ###
### Ceci est le fichier de test et vous n'êtes PAS supposé le modifier (sauf si vous voulez ajouter plus de tests) ###

# To run the test run the command pytest in the terminal being in the right folder (go see test instructions if needed)
# Pour rouler les tests avec pytest utilisez le terminal dans le bon folder et exécutez la commande pytest (voir le fichier donnée sur les tests)

from part_1 import part_1


def test_from_problem_description():
    assert part_1(["XX", "XX", "Cc", "Cc", "Cc", "CC", "Cc"]) == 25.0
    assert part_1(["XX", "XX", "XX", "sS", "ss", "Ss", "SS"]) == 18.75
    assert part_1(["XX", "XX", "XX", "XX", "Tt", "tt", "XX", "TT", "TT", "Tt", "Tt", "Tt", "XX", "TT", "tt"]) == 18.75

def test_from_description():
    assert part_1(["XX", "Cc", "cc"]) == 50.0


