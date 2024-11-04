### This is the test file and you are NOT supposed to modify it (unless you want to add more tests) ###
### Ceci est le fichier de test et vous n'êtes PAS supposé le modifier (sauf si vous voulez ajouter plus de tests) ###

# To run the test run the command pytest in the terminal being in the right folder (go see test instructions if needed)
# Pour rouler les tests avec pytest utilisez le terminal dans le bon folder et exécutez la commande pytest (voir le fichier donnée sur les tests)

from part5.part_5 import part_5


def test_from_problem_description():
    assert part_5("Tommy! Approche! Ici ils font des gros robots! They are for the TakTik 2025 competition. Kooooolll robots, that seems like so much fun! Kevin might also be interested to join the robotics team with us. Sweet, if both of you are joining, we will have a super good team and might win the event!") == [
"     T      ",
"    ↙ ↘     ",
"   ↙   ↘   ",
"  A     K  ",
" ↙ ↘   ↙ ↘ ",
"T   I K   S"
]
    

def test_lorem_ipsum():
    assert part_5("Lorem ipsum odor amet, consectetuer adipiscing elit. Penatibus quam montes ridiculus donec, curae dapibus. Volutpat leo ullamcorper maecenas at montes ante a. Enim vehicula senectus; risus elementum faucibus accumsan. Pretium sem nunc bibendum arcu dictum posuere maecenas rhoncus pulvinar. Est cras donec facilisis velit arcu convallis aptent neque neque. Varius sem sagittis vitae ex in euismod himenaeos.") == [
"     P      ",
                    "    ↙ ↘     ",
                    "   ↙   ↘   ",
                    "  V     E  ",
                    " ↙ ↘   ↙ ↘ ",
                    "V   L E   P"]