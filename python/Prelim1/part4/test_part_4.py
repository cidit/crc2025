### This is the test file and you are NOT supposed to modify it (unless you want to add more tests) ###
### Ceci est le fichier de test et vous n'êtes PAS supposé le modifier (sauf si vous voulez ajouter plus de tests) ###

# To run the test run the command pytest in the terminal being in the right folder (go see test instructions if needed)
# Pour rouler les tests avec pytest utilisez le terminal dans le bon folder et exécutez la commande pytest (voir le fichier donnée sur les tests)

from part4.part_4 import part_4


def test_from_problem_description():
    assert part_4(['u', 's', 'o', 'n', 'i', 'c', 'e']) == [
"     u     ",
"    ↙ ↘    ",
"   ↙   ↘   ",
"  s     o  ",
" ↙ ↘   ↙ ↘ ",
"n   i c   e"
]
    assert part_4(['x', 'u', 'p', 'l', 'e', 'f', 't']) == [
"     x     ",
"    ↙ ↘    ",
"   ↙   ↘   ",
"  u     p  ",
" ↙ ↘   ↙ ↘ ",
"l   e f   t"
]
    assert part_4(['o', 'e', 'n', 'h', 'a', 'u', 't']) == [
"     o     ",
"    ↙ ↘    ",
"   ↙   ↘   ",
"  e     n  ",
" ↙ ↘   ↙ ↘ ",
"h   a u   t"
]


def test_abc():
    assert part_4(['a', 'b', 'c', 'd', 'e', 'f', 'g']) == [
"     a     ",
"    ↙ ↘    ",
"   ↙   ↘   ",
"  b     c  ",
" ↙ ↘   ↙ ↘ ",
"d   e f   g"
]