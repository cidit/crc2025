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

##############################################
#                  Classes                   # 
##############################################
class Station:
    coords:tuple[int, int]
    customers:list[tuple[int, int]] = []
    nb_customers:int = 0

    def __init__(self, coords:tuple[int, int]):
        self.coords = coords
    
    def compare_to(self, other_customers:list[tuple[int, int]])->list[tuple[int, int]]:
        return set(self.customers) & set(other_customers)
    
    def set_customers(self, custs:list[tuple[int, int]]):
        self.customers = custs
        self.nb_customers = len(custs)
    
    def add_customer(self, customer:tuple[int, int]):
        self.customers.append(customer)
        self.nb_customers = len(self.customers)

    def remove_customer(self, customer:tuple[int, int]):
        self.customers.remove(customer)
        self.nb_customers = len(self.customers)


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
    solution:list[list[tuple[int, int]]] = []
    stations:list[Station] = []
    customers:list[tuple[int, int]] = []
    remaining_customers:list[tuple[int, int]] = []

    #Find stations and customers
    for x in range(n):
        for y in range(n):
            if map[x][y] == 1:
                stations.append(Station(coords=(x, y)))
            elif map[x][y] == 2:
                customers.append((x, y))

    #Sort stations
    stations.sort(key=lambda stat: stat.coords)
    remaining_customers = list(customers)
    # print("NB Stations:", len(stations))
    # print("Available Customers:", customers)


    #Find the 5 best customers for each station
    for stat in stations:
        #Sort customers based on distance and take the first 5
        customers.sort(key=lambda cust: distance_manhattan(stat.coords, cust))
        stat.set_customers(list(customers[:5]))

        # print("Station:", stat.coords, " Used Customers:", stat.customers)
        #Remove used customers from remaining list
        for cust in stat.customers:
            try:
                #print("Removing: ", cust)
                remaining_customers.remove(cust)
            except:
                # print("\tDid not find:", cust)
                pass
        
        # print("\tRemaining Cust:", remaining_customers)


    #Check if customer is used more than once
    for i in range(len(stations)):
        for j in range(len(stations)-i-1):
            matches = stations[i].compare_to(stations[i+j+1].customers)
            if matches:
                # print("Matches!")
                #Found similar, remove from furthest station
                for match in matches:
                    # print("\tRemoving customer", match, "from station", stat.coords)
                    stations[i].remove_customer(match) if distance_manhattan(match, stations[i].coords) > distance_manhattan(match, stations[i+j+1].coords) else stations[i+j+1].remove_customer(match)


    #Do we have extra customers?
    if not len(remaining_customers) == 0:
        # print("Extras!")
        #Assign every remaining customer to a station
        for cust in remaining_customers:
            #Find the best station for this customer, only dist
            shortest_dist = 99999999
            best_stat_index = 0
            for stat_index in range(len(stations)):
                dist_man = distance_manhattan(cust, stations[stat_index].coords)
                if dist_man < shortest_dist and stations[stat_index].nb_customers < 5:
                    best_stat_index = stat_index
        
            #Add customer to station
            # print("\tAdding ", cust, " to the station ", stations[best_stat_index].coords)
            stations[best_stat_index].add_customer(cust)
    

    #Generate Solution from Stations list
    # print("Generating Solution...")
    solution = [stat.customers for stat in stations]

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
    customers = []

    #Find customers
    for x in range(n):
        for y in range(n):
            if map[x][y] == 2:
                customers.append((x, y))

    return customers
