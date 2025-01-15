### This is the file that will benchmark your results, you don't have to modify it! (except line 15) ###
### Ceci est le fichier qui va faire les tests de performance, vous n'avez pas à le modifier! (sauf la ligne 15) ###
import time
import random

from prelim4 import solve

from game_logic import create_map
from game_logic import is_valid_solution
from game_logic import get_score
from game_logic import view_stations
from game_logic import view_solution

def benchmark():
    test_with_one_size = 100 ### put the value of n you want to try your algorithm with or use 0 for a full benchmark




    if test_with_one_size >= 5:
        return test_one_size(test_with_one_size)
    
    values_to_test = [10, 25, 50, 100, 250, 500, 1000, 2500, 5000, 10000]
    score_per_value = {}
    tests_per_value = 3

    for n in values_to_test:
        for i in range(tests_per_value):
            city_map = create_map(n, random.randint(i, 10000))
            start_time = time.time() * 1000
            solution = solve(city_map, n)
            end_time = time.time() * 1000

            if not is_valid_solution(city_map, solution):
                print('This solution is invalid!!!')
            if end_time - start_time > 60000:
                print('solution took too much time!!!')

            #print('time to solve size', n, ' is ', (end_time-start_time), 'ms')
            score_per_value[n] = get_score(city_map, solution)/tests_per_value + (score_per_value[n] if n in score_per_value else 0)
    
    for n in score_per_value:
        print("For n =", n, " your average risk score is ", round(score_per_value[n], 2))
    return

def test_one_size(size: int):
    city_map = create_map(size, 100)#random.randint(0, 10000)
    view_stations(city_map)

    start_time = time.time() * 1000
    solution = solve(city_map, size)
    end_time = time.time() * 1000

    view_solution(city_map, solution)
    score = round(get_score(city_map, solution), 2)
    print("For n =", size, " your time to solve is", round(end_time-start_time, 2), "ms and your risk score is ", round(score, 2))

benchmark()