class Unit:
    def __init__(self, name: str, attack_value: int, defense_value: int, priority: int, health: int):
        self.name = name
        self.attack_value = attack_value
        self.defense_value = defense_value
        self.health = health
        self.priority = priority
        self.is_alive = True
    
    def attack(self, target: "Unit") -> None:
        target.defend(self.attack_value)
        
    def defend(self, attack: int) -> None:
        self.health -= attack - self.defense_value
        self.is_alive =  self.health > 0


def combat(army1: list[Unit], army2: list[Unit]) -> list[Unit]:

    while sum([u.health for u in army1]) > 0 and sum([u.health for u in army2]) > 0:
        army1.sort(key= lambda u: (not u.is_alive, u.priority, u.health))
        army2.sort(key= lambda u: (not u.is_alive, u.priority, u.health))

        #Find priority
        p1 = 0
        for i in range(len(army1)):
            if not army1[i].is_alive:
                continue
            p1 = army1[i].priority
            break
        
        #Attack
        for i in range(len(army1)):
            if not army1[i].priority == p1:
                break

            army1[i].attack(army2[0])
            army2.sort(key= lambda u: (not u.is_alive, u.priority, u.health))


        #Find priority
        p2 = 0
        for i in range(len(army1)):
            if not army2[i].is_alive:
                continue
            p2 = army2[i].priority
            break

        #Attack
        for i in range(len(army2)):
            if not army2[i].priority == p2:
                break

            army2[i].attack(army1[0])
            army1.sort(key= lambda u: (not u.is_alive, u.priority, u.health))
    ### Vous devez implementer la logique du combat tour par tour ###
    ### You need to implement the turn based combat logic ###
    pass

def ragnarok(combats: list[tuple[str]], armies: dict[list[Unit]]) -> list[str]:
    for c in combats:
        combat(armies[c[0]], armies[c[1]])

    axis:list = []
    allies:list = []
    armyAllies = []
    armyAxis = []
    for army in armies:
        if sum([u.health for u in armies[army]]) <= 0:
            continue

        if army == "Odin" or army == "Thor" or army == "Freya":
            allies.append(army)
            armyAllies.extend(armies[army])
        elif army == "Loki" or army == "Hel" or army == "Fenrir":
            axis.append(army)
            armyAxis.extend(armies[army])

    


    h1 = sum([u.health for u in armyAllies])
    h2 = sum([u.health for u in armyAxis])

    if h1 < h2:
        combat(armyAllies, armyAxis)
    else:
        combat(armyAxis, armyAllies)
    return allies if sum([u.health for u in armyAllies]) > 0 else axis


    ### Vous devez implementer les combats du ragnarok ###
    ### You have to implement the combats of the ragnarok ###