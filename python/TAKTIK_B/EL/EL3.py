# Earth is cracking (70 points) #

import itertools

def pointInRect(point,rect):
    x1, y1, x2, y2 = rect
    x, y = point
    if (x1 < x and x < x2):
        if (y1 < y and y < y2):
            return True
    return False

def intersection(
    l1: tuple[tuple[int, int], tuple[int, int]],
    l2: tuple[tuple[int, int], tuple[int, int]]
):
    (x1, y1), (x2, y2) = l1
    (x3, y3), (x4, y4) = l2
    
    px = ((x1*y2-y1*x2)*(x3-x4)-(x1-x2)*(x3*y4-y3*x4))/((x1-x2)*(y3-y4)-(y1-y2)*(x3-x4))
    py = ((x1*y2-y1*x2)*(y3-y4)-(y1-y2)*(x3*y4-y3*x4))/((x1-x2)*(y3-y4)-(y1-y2)*(x3-x4))
    
    return (px, py)
    
def variation(segment: tuple[tuple[int, int], tuple[int, int]]):
    (x1, y1), (x2, y2) = segment
    return (y2-y1)/(x2-x1)

def solve(
    coins: tuple[tuple[int, int], tuple[int, int]], 
    craques: list[tuple[tuple[int, int], tuple[int, int]]]
    ) -> int:
    
    print("le dernier test case est incorrecte")
    
    rx1, ry1 = coins[0]
    rx2, ry2 = coins[1]
    rect = (rx1, ry1, rx2, ry2)

    zones = 1
    _intersection = []
    
    for i, c1 in enumerate(craques):
        zones += 1
        for c2 in craques[:i]:
            if variation(c1) == variation(c2):
                # no intersection
                continue
            (x, y) = intersection(c1, c2)
            if pointInRect((x, y), rect) and (x, y) not in _intersection:
                zones+=1
                _intersection.append((x, y))
    
    return zones
            
    
    
    # intersectionos = []
    # nb_pieces = len(craques)
   
    # for c1, c2 in itertools.permutations(craques, 2):
    #     (px, py) = intersection(c1, c2)
    #     rx1, ry1 = coins[0]
    #     rx2, ry2 = coins[1]
        
    #     if pointInRect((px, py), (rx1, ry1, rx2, ry2)) and (px, py) not in intersectionos:
    #         intersectionos.append((px, py))
            
    # return len(intersectionos)+1
   
