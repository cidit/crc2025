### This is the game logic file, you are NOT supposed to modify it ###
### Ceci est le fichier de logique du jeu, vous n'êtes PAS supposé le modifier ###

import random
import matplotlib.pyplot as plt
import matplotlib as mpl

from prelim4 import distance_manhattan
from prelim4 import get_stations

c_per_s = 5

def create_map(n: int, seed: int) -> list[list[int]]:
    random.seed(seed)
    number_stations = n // c_per_s if n >= c_per_s else 1
    map = [[0 for _ in range(n)] for _ in range(n)]
    for _ in range(n):
        x = random.randint(0, n-1)
        y = random.randint(0, n-1)
        while map[x][y] != 0:
            x = random.randint(0, n-1)
            y = random.randint(0, n-1)
        map[x][y] = 2
    
    for _ in range(number_stations):
        x = random.randint(0, n-1)
        y = random.randint(0, n-1)
        while map[x][y] != 0:
            x = random.randint(0, n-1)
            y = random.randint(0, n-1)
        map[x][y] = 1
    return map

def is_valid_solution(original_map: list[list[int]], solution: list[list[tuple[int, int]]]) -> bool:
    n = len(original_map)
    if len(solution) != n // c_per_s:
        print("You don't have the right amount of stations! You should have ", n//c_per_s, " and you have ", len(solution), " stations.")
        return False
    all_cities = []
    for section in solution:
        all_cities.extend(section)
    for i in range(n):
        for j in range(n):
            if original_map[i][j] == 2:
                if (i, j) not in all_cities:
                    print(i, j)
                    print("You have forgetten a city!")
                    return False
                else:
                    all_cities.remove((i, j))
    if len(all_cities) > 0:
        print("You have some unknown cities in your solution!")
        return False
    return True

def size_score(solution: list[list[tuple[int,int]]]) -> int:
    """Calculates the part of the score associated to fire stations having the wrong size.
    It is the square of the difference between the wanted number of cities and the 
    current number of cities for every fire stations."""
    size_penality = 0
    for fire_station in solution:
        size_penality += (len(fire_station) - c_per_s)**2
    print('size:', size_penality)
    return size_penality

def distance_score(original_map: list[list[int]], solution: list[list[tuple[int,int]]]) -> float:
    """Calculates the part of the score associated to the distance between cities and the fire station associated."""
    distance_penalty = 0
    n = len(original_map)
    stations = get_stations(original_map, n)
    for i, district in enumerate(solution):
        for city in district:
            distance_penalty += distance_manhattan(city, stations[i])
    print('distance:', distance_penalty/n)
    return distance_penalty / n

def get_score(original_map: list[list[int]], solution: list[list[tuple[int,int]]]) -> float:
    return round(size_score(solution) + distance_score(original_map, solution), 2)

def view_stations(map: list[list[int]]):
    colormap = (mpl.colors.ListedColormap(['white', 'red', 'blue']))
    plt.figure(figsize=(12,12))
    plt.imshow(map, cmap=colormap)
    plt.show()

def view_solution(map: list[list[int]], solution: list[list[tuple[int,int]]]):
    n = len(map)
    tab20_white = [(1, 1, 1), (1, 0, 0)]
    tab20_white.extend(mpl.cm.get_cmap('tab20', len(solution)).colors)
    colormap = mpl.colors.ListedColormap(tab20_white)

    colors = [[0 for _ in range(n)] for _ in range(n)]
    for i, district in enumerate(solution):
        for city in district:
            colors[city[0]][city[1]] = i+2
    for i, row in enumerate(map):
        for j, square in enumerate(row):
            if square == 1:
                colors[i][j] = 1
                
    plt.figure(figsize=(12,12))
    plt.imshow(colors, cmap=colormap)
    plt.show()

