### This is the test file and you are NOT supposed to modify it (unless you want to add more tests) ###
### Ceci est le fichier de test et vous n'êtes PAS supposé le modifier (sauf si vous voulez ajouter plus de tests) ###

# To run the test run the command pytest in the terminal being in the right folder (go see test instructions if needed)
# Pour rouler les tests avec pytest utilisez le terminal dans le bon folder et exécutez la commande pytest (voir le fichier donnée sur les tests)

from part_3 import part_3


def test_from_problem_description():
    systolic = 120
    diastolic = 90
    heart_rate = 120
    assert part_3(systolic, diastolic, heart_rate) == [
    " _            _            _            _           ",
    "| |          | |          | |          | |          ",
    "| |          | |          | |          | |          ",
    "| |          | |          | |          | |          ",
    "| |          | |          | |          | |          ",
    "| |          | |          | |          | |          ",
    "| |_   ______| |_   ______| |_   ______| |_   ______",
    "    | |          | |          | |          | |      ",
    "    | |          | |          | |          | |      ",
    "    | |          | |          | |          | |      ",
    "    |_|          |_|          |_|          |_|      "
    ]
    
    systolic = 108
    diastolic = 77
    heart_rate = 105
    assert part_3(systolic, diastolic, heart_rate) == [
    " _              _              _              _             ",
    "| |            | |            | |            | |            ",
    "| |            | |            | |            | |            ",
    "| |            | |            | |            | |            ",
    "| |            | |            | |            | |            ",
    "| |_   ________| |_   ________| |_   ________| |_   ________",
    "    | |            | |            | |            | |        ",
    "    | |            | |            | |            | |        ",
    "    |_|            |_|            |_|            |_|        "
    ]

    systolic = 100
    diastolic = 92
    heart_rate = 135
    assert part_3(systolic, diastolic, heart_rate) == [
    " _          _          _          _         ",
    "| |        | |        | |        | |        ",
    "| |        | |        | |        | |        ",
    "| |        | |        | |        | |        ",
    "| |        | |        | |        | |        ",
    "| |_   ____| |_   ____| |_   ____| |_   ____",
    "    | |        | |        | |        | |    ",
    "    | |        | |        | |        | |    ",
    "    | |        | |        | |        | |    ",
    "    |_|        |_|        |_|        |_|    "
    ]

