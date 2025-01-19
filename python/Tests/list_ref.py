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


class Customer:
    coords:tuple[int, int]
    assignements:list[Station] = []
    nb_assign:int = 0

    def __init__(self, coords:tuple[int, int]):
        self.coords = coords

    def add_assign(self, station:tuple[int, int]):
        self.assignements.append(station)
        self.nb_assign = len(self.assignements)

    def remove_customer(self, station:tuple[int, int]):
        self.assignements.remove(station)
        self.nb_assign = len(self.assignements)



#--------------------------------------------------
stations = [Station((2, 3))]
customers = [Customer((6, 5))]

customers[0].add_assign(stations[0])
print(stations[0].coords)
print(customers[0].assignements[0].coords)

stations[0].coords = (3, 4)
print(stations[0].coords)
print(customers[0].assignements[0].coords)