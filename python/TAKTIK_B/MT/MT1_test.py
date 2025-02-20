from MT1 import solve

def test_document1():
    assert(solve([0, 2, 4, 8, 3, 5, 1]) == ['L', 'L'])

def test_document2():
    assert(solve([0, 2, 4, 8, 3, 5, 1, 1, 2, 2, 3, 9, 8, 7, 5]) == ['R', 'L', 'L'])

def test_document3():
    assert(solve([0, 6, 4, 2, 5, 8, 1]) == ['R', 'L'])