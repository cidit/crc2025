### This is the test file and you are NOT supposed to modify it (unless you want to add more tests) ###
### Ceci est le fichier de test et vous n'êtes PAS supposé le modifier (sauf si vous voulez ajouter plus de tests) ###

# To run the test run the command pytest in the terminal being in the right folder (go see test instructions if needed)
# Pour rouler les tests avec pytest utilisez le terminal dans le bon folder et exécutez la commande pytest (voir le fichier donnée sur les tests)

from part3.part_3 import part_3


def test_from_problem_description():
    assert part_3([2, 4, 6, 5, 3, 1, 7], ['b', 'a', 'c', 'f', 'e', 'd', 'g']) == ['a', 'b', 'c', 'd', 'e', 'f', 'g']
    assert part_3([31, 44, 14, 33, 43, 26, 57], ['i', 'o', 'n', 'u', 'c', 's', 'e']) == ['u', 's', 'o', 'n', 'i', 'c', 'e']
    assert part_3([14, 41, 10, 59, 99, 109, 90], ['u', 'e', 'l', 'x', 'p', 't', 'f']) == ['x', 'u', 'p', 'l', 'e', 'f', 't']

def test_supplementaire():
    assert part_3([71, 70, 42, 61, 23, 27, 29], ['t', 'n', 'o', 'u', 'h', 'e', 'a']) == ['o', 'e', 'n', 'h', 'a', 'u', 't']
