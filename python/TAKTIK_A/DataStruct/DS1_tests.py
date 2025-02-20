from DS1 import solve

def test_document1():
    assert(solve(['A32', 'A42', 'R', 'A7', 'R', 'A21', 'A20', 'R', 'A53']) == [[53], [20, 21]])

def test_document2():
    assert(solve(['A3', 'A5', 'A2', 'R', 'A7', 'R', 'A4', 'R', 'R', 'A9']) == [[9], [4]])


def test_document3():
    assert(solve(['A61', 'A80', 'A7', 'A9', 'A20', 'R', 'A53']) == [[53], [20, 9, 7, 80]])

