from SC1 import solve

def test_document1():
    assert(solve(12, 8) == 0.58)

def test_document2():
    assert(solve(10, 10) == 0.6)

def test_document3():
    assert(solve(120, 80) == 0.06)

