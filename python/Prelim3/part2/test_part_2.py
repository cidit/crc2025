### This is the test file and you are NOT supposed to modify it (unless you want to add more tests) ###
### Ceci est le fichier de test et vous n'êtes PAS supposé le modifier (sauf si vous voulez ajouter plus de tests) ###

# To run the test run the command pytest in the terminal being in the right folder (go see test instructions if needed)
# Pour rouler les tests avec pytest utilisez le terminal dans le bon folder et exécutez la commande pytest (voir le fichier donnée sur les tests)

from part_2 import part_2


def test_from_problem_description():
    arrivals = 80
    mu = 50
    c = 2
    assert part_2(arrivals, mu, c) == 3.33
    
    arrivals = 42
    mu = 8
    c = 7
    assert part_2(arrivals, mu, c) == 9.17

    arrivals = 6
    mu = 3
    c = 4
    assert part_2(arrivals, mu, c) == 21.7


def test_supp():
    arrivals = 20
    mu = 5
    c = 5
    assert part_2(arrivals, mu, c) == 18.6

