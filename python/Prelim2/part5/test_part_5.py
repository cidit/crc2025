### This is the test file and you are NOT supposed to modify it (unless you want to add more tests) ###
### Ceci est le fichier de test et vous n'êtes PAS supposé le modifier (sauf si vous voulez ajouter plus de tests) ###

# To run the test run the command pytest in the terminal being in the right folder (go see test instructions if needed)
# Pour rouler les tests avec pytest utilisez le terminal dans le bon folder et exécutez la commande pytest (voir le fichier donnée sur les tests)

from part_5 import part_5


def test_from_problem_description():
    assert part_5(h = 160, d = 140, angle = 35) == 23.56
    assert part_5(h = 18, d = 100, angle = 40) == 28.64
    assert part_5(h = 28, d = 85, angle = 45) == 25.04
    

def test_supp():
    assert part_5(h = 45, d = 145, angle = 30) == 32.68