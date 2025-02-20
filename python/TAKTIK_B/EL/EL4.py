# forest fire (40 points) #
#
# 0 = vierge
# 1 = allumé
# 2 = enflammé
# 3 = brulé

def solve(feux: list[tuple[int, int]], debut: str) -> str:
    map = [[0 for col in range(20)] for row in range(20)]

    #add init enflammé
    for feu in feux:
        map[feu[0]][feu[1]] = 2

    # print_map(map)
    # print("\n")

    brule = True
    nbH = 0
    while brule:
        newMap = [[0 for col in range(20)] for row in range(20)]
        change = False
        for i in range(20):
            for j in range(20):
                num = map[i][j]
               
                if num == 2 or num == 3:
                    #devient bruler? on l'est déja?
                    newMap[i][j] = 3
                    continue

                #On trouve les cases a coté
                adj = []
                if not i == 0:
                    adj.append(map[i-1][j])
                if not i == 19:
                    adj.append(map[i+1][j])
                if not j == 19:
                    adj.append(map[i][j+1])
                if not j == 0:
                    adj.append(map[i][j-1])

                #nb case enflammé
                numEnf = 0
                for case in adj:
                    if case == 2:
                        numEnf += 1

                #Check state
                if num == 0:
                    #Vierge
                    if numEnf == 1:
                        newMap[i][j] = 1
                        change = True
                    elif numEnf >= 2:
                        newMap[i][j] = 2
                        change = True

                elif num == 1:
                    #allumé
                    #if numEnf >= 2:
                    newMap[i][j] = 2
                    change = True
        map = newMap.copy()
        # print_map(map)
        # print("\n")
        nbH += 1
        if not change:
            brule = False

    day = int(debut[0:debut.index(" ")])
    hour = int(debut[debut.index(",")+2:debut.index(":")]) + nbH

    while hour >= 24:
        hour -= 24
        day += 1

    #format
    date = str(day)+debut[debut.index(" "):debut.index(",")+2]+str(hour)+debut[debut.index(":"):]
    return date


def print_map(map):
    for row in map:
        print(row)    


    