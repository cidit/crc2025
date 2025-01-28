import time


def distance_manhattan(city_a: tuple[int, int], city_b: tuple[int, int]) -> int:
    """Part_1: Find Manhattan Distance"""
    distance = 0

    x = city_b[0] - city_a[0]
    y = city_b[1] - city_a[1]
    distance = abs(x) + abs(y)

    return distance

def solve(map: list[list[int]], n: int) -> list[list[tuple[int, int]]]:
    #Variables needed in function
    stations = []
    cities = []

    start_time = time.process_time()*1000.0
    #Find stations and cities
    
    stat_index = 0
    for y in range(n):
        for x in range(n):
            val = map[x][y]
            if val == 0:
                continue
            elif val == 1:
                stations.append((x, y, stat_index))
                stat_index += 1
            elif val == 2:
                cities.append((x, y))
    print("Search:", time.process_time()*1000.0 - start_time)

    #Génére empty solution
    nb_stations = len(stations)
    solution = [[] for _ in range(nb_stations)]
    print("Gen Solu:", time.process_time()*1000.0 - start_time)

    #Calculating weight
    weights = [0]*nb_stations
    for station in stations:        
        for city in cities:
            dist = distance_manhattan(station, city)
            weights[station[2]] += dist**2

    print("Weight:", time.process_time()*1000.0 - start_time)

    #Sorting
    stations.sort(key=lambda stat: weights[stat[2]])

    print("Sorting:", time.process_time()*1000.0 - start_time)

    #Assining
    for i in range(5):
        stat_num = 0
        for station in stations:
            #print("Station #", stat_num)
            short_dist = 9999999999
            close_city = None

            for city in cities:
                dist = distance_manhattan(station, city)

                if dist < short_dist:
                    short_dist = dist
                    close_city = city

            solution[station[2]].append(city)
            cities.remove(close_city)
            #print("   Dist: ", short_dist)
            stat_num += 1

        print("Loop #", i, ":", time.process_time()*1000.0 - start_time)  
    
    return solution