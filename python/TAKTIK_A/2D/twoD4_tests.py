from twoD4 import solve

def test_document1():
    assert(solve((46, 100), (23, 50), [(30, 26, 46, 50), (23, 100, 46, 100)]) == True)
    
def test_document2():
    assert(solve((30, 30), (15, 5), [(10, 10, 10, 30),(5, 0, 5, 5), (20, 8, 30, 8)]) == False)
    
def test_document3():
    assert(solve((10, 10), (4, 56), [(8, 1, 8, 6)]) == True)
