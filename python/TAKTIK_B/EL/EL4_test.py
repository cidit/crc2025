from EL4 import solve

def test_document1():
    assert(solve([(8,2),(3,3)], "20 janvier, 8:57") == "22 janvier, 6:57")

def test_document2():
    assert(solve([(0,19)], "5 mars, 11:11") == "7 mars, 21:11")

def test_document3():
    assert(solve([(9,9)], "7 décembre, 17:00") == "9 décembre, 0:00")

def test_document4():
    assert(solve([(17,2),(8,11),(13,7)], "22 août, 9:23") == "23 août, 13:23")