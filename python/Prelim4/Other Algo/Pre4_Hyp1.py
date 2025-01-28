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

##############################################
#                  Classes                   # 
##############################################
class Station:
    coords:tuple[int, int]
    cities:list[tuple[int, int]] = []
    nb_cities:int = 0

    def __init__(self, coords:tuple[int, int]):
        self.coords = coords
    
    def compare_to(self, other_cities:list[tuple[int, int]])->list[tuple[int, int]]:
        return set(self.cities) & set(other_cities)
    
    def set_cities(self, cs:list[tuple[int, int]]):
        self.cities = cs
        self.nb_cities = len(cs)
    
    def add_city(self, city:tuple[int, int]):
        if len(self.cities) == 0:
            self.cities = [city]
        else:
            self.cities.append(city)
        self.nb_cities = len(self.cities)

    def remove_city(self, city:tuple[int, int]):
        self.cities.remove(city)
        self.nb_cities = len(self.cities)


class City:
    coords:tuple[int, int]
    stations:list[Station]
    nb_stations_radius:int = 0
    stations_weight:int = 0

    assignements:list[Station] = []
    nb_assign:int = 0

    def __init__(self, coords:tuple[int, int]):
        self.coords = coords

    def add_assign(self, station:Station):
        if len(self.assignements) == 0:
            self.assignements = [station]
        else:
            self.assignements.append(station)
        station.add_city(self.coords)
        self.nb_assign = len(self.assignements)

    def remove_assign(self, station:Station):
        self.assignements.remove(station)
        station.remove_city(self.coords)
        self.nb_assign = len(self.assignements)

    def closest_station(self)->Station:
        man_dist = [distance_manhattan(self.coords, stat.coords) for stat in self.assignements]
        return self.assignements[man_dist.index(min(man_dist))]
    
    def keep_station(self, station:Station):
        to_remove = []
        for stat in self.assignements:
            #print("\t\tCurrent Stat", stat)
            if stat == station:
                #print("\t\tKeeping")
                continue
            #print("\t\tRemoving")
            to_remove.append(stat)
        
        #Must be in a seperate loop, if removed while looping, previous loop gets shortened
        for r in to_remove:
            self.remove_assign(r)
    

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

    #Sort stations (Needed by prelim requirements)
    stations.sort(key=lambda stat: stat.coords)


    # Pour chaque ville, combien de caserne dans un rayon donnée (selon taille grille)
    # Commence par assigner les villes isoler

    radius = n * 0.25
    print("Radius:", radius)

    #Sorting stations in
    for city in cities:
        city.stations = stations.copy()
        city.stations.sort(key=lambda stat: distance_manhattan(stat.coords, city.coords))
        
        for station in city.stations:
            dist = distance_manhattan(station.coords, city.coords)
            city.stations_weight += dist**2
            # if distance_manhattan(station.coords, city.coords) > radius:
            #     break

            # city.nb_stations_radius += 1

    cities.sort(key=lambda city: city.stations_weight, reverse=True)

    city_num = 0
    for city in cities:
        print("City #", city_num)
        print("   Nb Station in radius", city.nb_stations_radius)
        for station in city.stations:
            if station.nb_cities >= 5:
                print("   Station full")
                continue

            city.add_assign(station)
            print("   Dist: ", distance_manhattan(station.coords, city.coords))
            break
        city_num += 1
    

    #Generate Solution from Stations list
    # print("Generating Solution...")
    solution = [stat.cities for stat in stations]

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
def get_cities(map: list[list[int]], n: int) -> list[tuple[int, int]]:
    cities = []

    #Find cities
    for x in range(n):
        for y in range(n):
            if map[x][y] == 2:
                cities.append((x, y))

    return cities




#------------- Best Yet -------------------
# For n = 10  your average risk score is  5.47
# For n = 25  your average risk score is  9.8
# For n = 50  your average risk score is  17.03
# For n = 100  your average risk score is  26.55
# For n = 250  your average risk score is  43.75
# For n = 500  your average risk score is  89.68
# For n = 1000  your average risk score is  116.58
# For n = 2500  your average risk score is  208.72
# For n = 5000  your average risk score is  373.51
# For n = 10000  your average risk score is  675.7