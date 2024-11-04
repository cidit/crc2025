### This is the test file and you are NOT supposed to modify it (unless you want to add more tests) ###
### Ceci est le fichier de test et vous n'êtes PAS supposé le modifier (sauf si vous voulez ajouter plus de tests) ###

# To run the test run the command pytest in the terminal being in the right folder (go see test instructions if needed)
# Pour rouler les tests avec pytest utilisez le terminal dans le bon folder et exécutez la commande pytest (voir le fichier donnée sur les tests)

from part2.part_2 import part_2


def test_from_problem_description():
    assert part_2("My cat always finds a way to sneak inside! Every bird loves flying under the bright sky. Fred enjoys cycling with his dog every morning.") == [270, 312, 350]
    assert part_2("Pourquoi est-ce que j'ai le réflexe de regarder mes mains lorsque j'échappe un frisbee?") == [1216]
    assert part_2("Maybe the elevator will arrive faster if I keep pressing the button. Je me suis lavé les mains après avoir cuisiné, mais elles sentent encore l'ail.") == [759, 986]

def test_1():
    assert part_2("Le!") == [1]