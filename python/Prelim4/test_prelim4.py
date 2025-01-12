### This is the test file and you are NOT supposed to modify it (unless you want to add more tests) ###
### Ceci est le fichier de test et vous n'êtes PAS supposé le modifier (sauf si vous voulez ajouter plus de tests) ###

# To run the test run the command pytest in the terminal being in the right folder (go see test instructions if needed)
# Pour rouler les tests avec pytest utilisez le terminal dans le bon folder et exécutez la commande pytest (voir le fichier donnée sur les tests)

from prelim4 import distance_manhattan
from prelim4 import get_stations
from prelim4 import get_customers

from game_logic import create_map

def test_distance():
    assert distance_manhattan([0, 0], [1, 5]) == 6
    assert distance_manhattan([20, 18], [14, 15]) == 9
    assert distance_manhattan([2, 3], [4, 5]) == 4

def test_get_stations():
    seed = 0
    city_5 = create_map(5, seed)
    print(city_5)
    assert get_stations(city_5, 5) == [(2, 1)]
    city_20 = create_map(20, seed)
    assert get_stations(city_20, 20) == [(0, 19), (2, 12), (15, 10), (17, 0)]

def test_get_customers():
    seed = 0
    city_5 = create_map(5, seed)
    assert get_customers(city_5, 5) == [(0, 2), (3, 2), (3, 3), (4, 1), (4, 3)]
    city_20 = create_map(20, seed)
    assert get_customers(city_20, 20) == [(1, 8), (2, 10), (3, 11), (3, 19), (8, 1), (8, 17), (9, 3), (9, 4), (12, 9), (12, 13), (13, 10), (14, 16), (15, 11), (15, 17), (16, 4), (16, 15), (17, 15), (18, 6), (19, 4), (19, 6)]

