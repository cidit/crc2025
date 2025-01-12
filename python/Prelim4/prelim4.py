# For your imports look at the rulebook to see what is allowed
# Pour les import permis allez vous référer au livre des règlements
"""prelim4.py
This is the template file for Prelim4.
Ceci est le fichier template pour le problème Prelim4.
"""

def distance_manhattan(city_a: tuple[int,int], city_b: tuple[int,int]) -> int:
    distance = 0

    x = city_b[0] - city_a[0]
    y = city_b[1] - city_a[1]
    distance = abs(x) + abs(y)

    return distance


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

def get_customers(map: list[list[int]], n: int) -> list[tuple[int, int]]:
    customers = []

    #Find customers
    for x in range(n):
        for y in range(n):
            if map[x][y] == 2:
                customers.append((x, y))

    return customers


def solve(map: list[list[int]], n: int) -> list[list[tuple[int, int]]]:
    solution = []
    
    #----- Optimization ideas -----
    #
    # Iterate only once for stations and customers 
    #

    #----- Algorithm Priority -----
    #   1. Nb customer per station
    #       - Even distribution is better: 444 > 552
    #
    #   2. Dist between customer and station
    #       - 
    
    stations = []
    customers = []

    #Find stations and customers
    for x in range(n):
        for y in range(n):
            if map[x][y] == 1:
                stations.append((x, y))
            elif map[x][y] == 2:
                customers.append((x, y))

    #Sort stations
    stations.sort()

    #Determine best nb of customer per station
    nb_station = len(stations)
    nb_customer = len(customers)
    cust_per_stat = int(nb_customer/nb_station) #Best nb cust per station
    remainder = nb_customer % nb_station #Station in extra

    print("Stations:", stations)
    print("Customers:", customers)
    print("Cust/Stat: ", cust_per_stat, " Remainder: ", remainder)

    #Create empty list for every station
    for _ in stations:
        cust_list = list() 
        solution.append(cust_list)


    # #Find the best customers for each station
    # for stat in stations:
    #     #Sort customers based on distance and take the first ones
    #     customers.sort(key=lambda cust: distance_manhattan(stat, cust))
    #     solution.append(list(customers[:cust_per_stat]))

    #     #Remove used customers from list
    #     customers = customers[cust_per_stat:]
    #     print("Remaining Cust: ", customers)

    # #Do we have extras
    # if not len(customers) == 0:
    #Assign every customer to a station
    for cust in customers:
        #Find the best station for this customer, only dist
        shortest_dist = 99999999
        best_stat_index = 0
        for stat_index in range(len(stations)):
            dist_man = distance_manhattan(cust, stations[stat_index])
            if dist_man < shortest_dist:
                best_stat_index = stat_index
    
        #Add customer to station
        solution[best_stat_index].append(cust)
    
    #print("solution: ", solution)

    return solution