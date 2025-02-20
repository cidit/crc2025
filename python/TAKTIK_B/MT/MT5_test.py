from MT5 import solve

def test_document1():
    assert(solve(distance=850, speed=60, time=8, offset=10) == "Okay")

def test_document2():
    assert(solve(distance=720, speed=65, time=9, offset=5) == "Danger")

def test_document3():
    assert(solve(distance=880, speed=55, time=4, offset=11) == "Perfect")
