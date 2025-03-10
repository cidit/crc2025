# For your imports look at the rulebook to see what is allowed
# Pour les import permis allez vous référer au livre des règlements
"""prelim4.py
This is the template file for Prelim4.
Ceci est le fichier template pour le problème Prelim4.
"""
#----- Optimization ideas -----
#
# In some cases it would be better to over and under stack
# 
#
# Notes: Nb of station is  n (size of map) / 5
#

#import time
# Search: 1734.375
# firstloop: 5343.75
# sort: 5343.75
# Loop # 0 : 7015.625
# Loop # 1 : 8421.875
# Loop # 2 : 9359.375
# Loop # 3 : 9765.625
# Loop # 4 : 10000.0
# Sort2: 10000.0
# Solution: 10000.0

##############################################
#                  Classes                   # 
##############################################
class Station:
    coords:tuple[int, int]
    cities:list[tuple[int, int]] = []
    weight:int = 0

    def __init__(self, coords:tuple[int, int]):
        self.coords = coords
    
    def add_city(self, city:tuple[int, int]):
        if len(self.cities) == 0:
            self.cities = [city]
        else:
            self.cities.append(city)

class City:
    coords:tuple[int, int]
    stations:list[Station]
    assignements:list[Station] = []

    def __init__(self, coords:tuple[int, int]):
        self.coords = coords

    def add_assign(self, station:Station):
        if len(self.assignements) == 0:
            self.assignements = [station]
        else:
            self.assignements.append(station)
        station.add_city(self.coords)
    

##############################################
#                  Solving                   # 
##############################################
def distance_manhattan(city_a: tuple[int,int], city_b: tuple[int,int]) -> int:
    """Part_1: Find Manhattan Distance"""
    distance = 0

    x = city_b[0] - city_a[0]
    y = city_b[1] - city_a[1]
    distance = abs(x) + abs(y)

    return distance

def solve(map: list[list[int]], n: int) -> list[list[tuple[int, int]]]:
    #Variables needed in function
    solution:list[list[tuple[int, int]]] = []
    stations:list[Station] = []
    cities:list[City] = []

    #start_time = time.process_time()*1000.0
    #Find stations and cities
    for x in range(n):
        for y in range(n):
            num = map[x][y]
            if num == 0:
                continue
            elif num == 1:
                stations.append(Station(coords=(x, y)))
            elif num == 2:
                cities.append(City((x, y)))


    # Pour chaque ville, combien de caserne dans un rayon donnée (selon taille grille)
    # Commence par assigner les villes isoler

    #radius = n * 0.25
    #print("Radius:", radius)

    #print("Search:", time.process_time()*1000.0 - start_time)
    for i in range(5):
        # Weight
        for station in stations:
            station.weight = 0
            cities.sort(key=lambda city: distance_manhattan(station.coords, city.coords))
            for city in cities[:10]:
                dist = distance_manhattan(station.coords, city.coords)
                station.weight += dist**2

        #print("Weight:", time.process_time()*1000.0 - start_time)

        stations.sort(key=lambda station: station.weight)

        #print("Sort:", time.process_time()*1000.0 - start_time)
    
        stat_num = 0
        for station in stations:
            #print("Station #", stat_num)
            short_dist = 9999999999
            close_city = None

            for city in cities:
                dist = distance_manhattan(station.coords, city.coords)

                if dist < short_dist:
                    short_dist = dist
                    close_city = city

            close_city.add_assign(station)
            cities.remove(close_city)
            #print("   Dist: ", short_dist)
            stat_num += 1

        #print("Loop #", i, ":", time.process_time()*1000.0 - start_time)    

    #Sort stations (Needed by prelim requirements)
    stations.sort(key=lambda stat: stat.coords)

    #print("Sort2:", time.process_time()*1000.0 - start_time)    

    #Generate Solution from Stations list
    # print("Generating Solution...")
    solution = [stat.cities for stat in stations]
    #print("Solution:", time.process_time()*1000.0 - start_time)    

    # print("Solution:")
    # print(*solution, sep='\n')
    return solution




##############################################
#                Other Parts                 # 
##############################################
#
# Part_2
#
def get_stations(map: list[list[int]], n: int) -> list[tuple[int, int]]:
    stations = []

    #Find stations
    for x in range(n):
        for y in range(n):
            if map[x][y] == 1:
                stations.append((x, y))

    #Sort stations
    stations.sort()

    return stations

#
# Part_3
#
def get_customers(map: list[list[int]], n: int) -> list[tuple[int, int]]:
    cities = []

    #Find cities
    for x in range(n):
        for y in range(n):
            if map[x][y] == 2:
                cities.append((x, y))

    return cities




#------------- Best Yet -------------------
# For n = 10  your average risk score is  4.7
# For n = 25  your average risk score is  9.65
# For n = 50  your average risk score is  17.19
# For n = 100  your average risk score is  29.92
# For n = 250  your average risk score is  46.28
# For n = 500  your average risk score is  63.02
# For n = 1000  your average risk score is  116.51
# For n = 2500  your average risk score is  191.17
# For n = 5000  your average risk score is  316.08
# For n = 10000  your average risk score is  435.38


# For n = 10  your average risk score is  5.13
# For n = 25  your average risk score is  10.12
# For n = 50  your average risk score is  13.98
# For n = 100  your average risk score is  25.69
# For n = 250  your average risk score is  46.68
# For n = 500  your average risk score is  71.99
# For n = 1000  your average risk score is  105.34
# For n = 2500  your average risk score is  171.98
# For n = 5000  your average risk score is  268.06
# For n = 10000  your average risk score is  422.43