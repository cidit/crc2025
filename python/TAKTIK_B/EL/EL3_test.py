from EL3 import solve

def test_document1():
    assert(solve(((0,0),(6,4)), [((0,0),(6,4)),((2,0),(4,4)),((0,4),(6,0)),((0,3),(6,3))]) == 10)

def test_document2():
    assert(solve(((0,0),(2,4)), [((0,4),(2,3)),((1,0),(2,3)),((0,4),(1,0)),((0,2),(1,4)),((0,3),(2,0)),((0,1),(2,1))]) == 14)

def test_document3():
    assert(solve(((0,0),(4,3)), [((4,1),(2,3)),((1,0),(2,3)),((0,3),(1,0)),((0,2),(4,1)),((0,3),(2,0)),((0,1),(4,2)),((2,0),(4,2)),((1,0),(4,2))]) == 23)