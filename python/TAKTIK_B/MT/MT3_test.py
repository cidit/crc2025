from MT3 import solve
import math

def dist(point1, point2):
    return math.sqrt((point1[0] - point2[0])**2 + (point1[1] - point2[1])**2)

def test_document1():
    sol = solve((0,0), (3,10), (((3,2),1),((2,7),1),((7,7),3)))
    best_known = 10.89
    target = best_known * 1.05
    total_dist = 0
    for i in range(len(sol)-1):
        total_dist += dist(sol[i], sol[i+1])
    assert(total_dist - target <= 0)


def test_document2():
    sol = solve((2,0), (9,4), (((6,2),2),((3,7),3),((8,6),1)))
    best_known = 9.60
    target = best_known * 1.05
    total_dist = 0
    for i in range(len(sol)-1):
        total_dist += dist(sol[i], sol[i+1])
    assert(total_dist - target <= 0)

def test_document3():
    sol = solve((6,9), (9,6), (((4,5),4),((9,8),1)))
    best_known = 4.24
    target = best_known * 1.05
    total_dist = 0
    for i in range(len(sol)-1):
        total_dist += dist(sol[i], sol[i+1])
    assert(total_dist - target <= 0)

def test_document4():
    sol = solve((2,9), (10,1), (((7,2),2),((4,7),2),((7,7),1),((9,6),1),((2,2),2)))
    best_known = 14.84
    target = best_known * 1.05
    total_dist = 0
    for i in range(len(sol)-1):
        total_dist += dist(sol[i], sol[i+1])
    assert(total_dist - target <= 0)

