from DS3 import solve

def test_document1():
        assert(solve([[2,3],[7,1]], [[-1,-2],[3,5]]) == [[7,11],[-4,-9]])

def test_document2():
        assert(solve([[1.1],[-0.7],[2.3]], [[5.4, -9.5]]) == [[5.94,-10.45], [-3.78,6.65], [12.42,-21.85]])

def test_document3():
        assert(solve([[5,8]], [[4,6]]) == "Impossible")

def test_document4():
        assert(solve([[1,-1],[-1,0],[1,1]], [[-1,-1,0],[1,0,-1]]) == [[-2,-1,1],[1,1,0],[0,-1,-1]])

def test_supp():
        assert(solve([[5,8],[1,3],[1,2]], [[1,2,3]]) == "Impossible")

