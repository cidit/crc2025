from SC3 import solve

def test_document1():
    assert(solve([355, 735]) == 5)

def test_document2():
    assert(solve([0, -90]) == 315)

def test_document3():
    assert(solve([-365, 395]) == 15)

