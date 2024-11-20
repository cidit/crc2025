### This is the test file and you are NOT supposed to modify it (unless you want to add more tests) ###
### Ceci est le fichier de test et vous n'êtes PAS supposé le modifier (sauf si vous voulez ajouter plus de tests) ###

# To run the test run the command pytest in the terminal being in the right folder (go see test instructions if needed)
# Pour rouler les tests avec pytest utilisez le terminal dans le bon folder et exécutez la commande pytest (voir le fichier donnée sur les tests)

from part_2 import part_2


def test_from_problem_description():
    d = 16
    h = 14
    b = 4
    m_cost = 0.42
    margin = 0.1
    assert part_2(d, h, b, m_cost, margin) == 48.56
    
    d = 18
    h = 16
    b = 3
    m_cost = 0.38
    margin = 0.15
    assert part_2(d, h, b, m_cost, margin) == 77.36

    d = 17
    h = 16
    b = 5
    m_cost = 0.40
    margin = 0.18
    assert part_2(d, h, b, m_cost, margin) == 102.75



def test_supp_hat():
    d = 15
    h = 13
    b = 3
    m_cost = 0.46
    margin = 0.15
    assert part_2(d, h, b, m_cost, margin) == 66.17