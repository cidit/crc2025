### Strange sort (30 points) ###

def solve(people: list[tuple[str, int]]) -> list[tuple[str, int]]:
    people.sort(key=lambda p: p[1])
    #print(people)

    for i in range(len(people)):
        if i == len(people)-1:
            continue 
        if people[i][0] > people[i+1][0]:
            people[i], people[i+1] = people[i+1], people[i]
        print(people)

    return people