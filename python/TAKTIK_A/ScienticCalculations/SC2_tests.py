from SC2 import solve
import math

def test_pi1():
    assert(abs(solve() - math.pi) < 0.05)

def test_pi2():
    assert(abs(solve() - math.pi) < 0.05)

def test_pi3():
    assert(abs(solve() - math.pi) < 0.05)
