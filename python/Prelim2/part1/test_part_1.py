### This is the test file and you are NOT supposed to modify it (unless you want to add more tests) ###
### Ceci est le fichier de test et vous n'êtes PAS supposé le modifier (sauf si vous voulez ajouter plus de tests) ###

# To run the test run the command pytest in the terminal being in the right folder (go see test instructions if needed)
# Pour rouler les tests avec pytest utilisez le terminal dans le bon folder et exécutez la commande pytest (voir le fichier donnée sur les tests)

from part_1 import part_1


def test_from_problem_description():
    assert part_1('4+11') == 411
    assert part_1('6*30') == 303030303030
    assert part_1('178+82') == 17882
    assert part_1('12*3') == 333333333333


def test_add():
    assert part_1('1+2') == 12
    assert part_1('2+1') == 21

def test_mult():
    assert part_1('1*2') == 2
    assert part_1('2*1') == 11

