from SC4 import solve

def test_document1():
    assert(solve(H=2.23, L=3, b=2.1, d=5) == 20.486)

def test_document2():
    assert(solve(H=4.1, L=6, b=3.2, d=10) == 1305.827)

def test_document3():
    assert(solve(H=3.4, L=4, b=1.2, d=18) == 19.609)
