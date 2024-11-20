### This is the test file and you are NOT supposed to modify it (unless you want to add more tests) ###
### Ceci est le fichier de test et vous n'êtes PAS supposé le modifier (sauf si vous voulez ajouter plus de tests) ###

# To run the test run the command pytest in the terminal being in the right folder (go see test instructions if needed)
# Pour rouler les tests avec pytest utilisez le terminal dans le bon folder et exécutez la commande pytest (voir le fichier donnée sur les tests)

from part_3 import part_3


def test_from_problem_description():
    assert part_3(["kitten", "sitting"]) == [
    " g^-^ ",
    "|@_@|",
    "s> <i"]
    assert part_3(["chatons", "adorables!"]) == [
    " ^-r!^ ",
    "|'_'| ",
    "ble<  "]

    assert part_3(["pun'ctu/ation", "pon~ctuat'ion"]) == [
    " ^-^' ",
    "|''| ",
    " o ~ "]

