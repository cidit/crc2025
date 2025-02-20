from DS2 import solve

def test_document1():
    assert(solve([("Justin", 18), ("Marie", 19), ("Judith", 22), ("Georges", 14), ("Zachary", 21)]
) == [("Georges", 14), ("Justin", 18), ("Marie", 19), ("Judith", 22), ("Zachary", 21)])

def test_document2():
    assert(solve([("Marc", 17), ("Samuel", 20), ("Ava", 22), ("Greg", 15)]
) == [("Greg", 15), ("Marc", 17), ("Ava", 22), ("Samuel", 20)])

def test_document3():
    assert(solve([("Felix", 25), ("Raphael", 24), ("Gabriel", 22), ("Francois", 23)]
) == [("Francois", 23), ("Gabriel", 22), ("Felix", 25), ("Raphael", 24)])
