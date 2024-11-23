### This is the test file and you are NOT supposed to modify it (unless you want to add more tests) ###
### Ceci est le fichier de test et vous n'êtes PAS supposé le modifier (sauf si vous voulez ajouter plus de tests) ###

# To run the test run the command pytest in the terminal being in the right folder (go see test instructions if needed)
# Pour rouler les tests avec pytest utilisez le terminal dans le bon folder et exécutez la commande pytest (voir le fichier donnée sur les tests)

from part_4 import part_4


def test_from_problem_description():
    assert part_4([0.5, -0.2, 0.5, -0.1], [40, 70, 70, 40]) == -0.37
    assert part_4([0.2, -0.2, 0.2, -0.2], [40, 70, 70, 40]) == 0.0
    assert part_4([0.5, -0.2, 0.5, -0.3, 0.4], [40, 70, 70, 40, 110]) == "On CHATvire!"


def test_valid():
    assert part_4([0.5, -0.5, 0.5, -0.3, -0.1], [40, 70, 70, 40, 110]) == 0.03

def test_outside():
    assert part_4([0.5, -0.5, -0.5, 0.2, -0.1], [40, 70, 70, 40, 110]) == "On CHATvire!"
