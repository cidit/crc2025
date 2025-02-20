from MT2 import solve

def test_document1():
    assert(solve(Vb = 4.6, Vc = 3.9, L = 1350) == "09m13s")

def test_document2():
    assert(solve(Vb = 5.1, Vc = 4.7, L = 2250) == "18m56s")

def test_document3():
    assert(solve(Vb = 3.1, Vc = 1.7, L = 800) == "05m09s")