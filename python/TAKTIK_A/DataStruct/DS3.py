### In the matrix (30 points) ###

def solve(m1: list[float], m2: list[float]):
    #Verif possible
    if len(m1[0]) != len(m2):
        return "Impossible"
    
    #Gen list
    solution = [[0 for col in range(len(m2[0]))] for row in range(len(m1))]
    print(solution)
    #Add value
    for row in range(len(solution)):
        for column in range(len(solution[0])):
            for i in range(len(m1[0])):
                print("m1", m1[row][i], " m2",m2[i][column])
                solution[row][column] += m1[row][i] * m2[i][column]
            print("Sol",solution[row][column])
                

    solution = [[round(solution[row][col], 2) for col in range(len(solution[0]))] for row in range(len(solution))]
    return solution
