### This is the test file and you are NOT supposed to modify it (unless you want to add more tests) ###
### Ceci est le fichier de test et vous n'êtes PAS supposé le modifier (sauf si vous voulez ajouter plus de tests) ###

# To run the test run the command pytest in the terminal being in the right folder (go see test instructions if needed)
# Pour rouler les tests avec pytest utilisez le terminal dans le bon folder et exécutez la commande pytest (voir le fichier donnée sur les tests)

from part1.part_1 import part_1


def test_from_problem_description():
    assert part_1("my cat always finds a way to sneak inside! Every bird loves flying under the bright sky. he enjoys cycling with his dog every morning.") == ['a', 'E', 'c']
    assert part_1("Samuel veut écrire du code en Python. ton script est trop cool! utilise-t-il des modules externes?") == ['P', 'c', 'd']
    assert part_1("Wait 1 second, he's not here anymore. je ne le vois plus! me neither, where did he go? je pense qu'il s'est perdu; cette ville est tellement belle.") == ['W', 'e', 'd', 'b']
    


def test_single_sentence():
    assert part_1('This sentence is the only one') == ['T']

def test_uppercase():
    assert part_1("YOU CAN HANDLE UPPERCASE!") == ['A']

