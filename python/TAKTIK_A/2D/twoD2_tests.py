from twoD2 import solve

def test_document1():
    assert(solve(14, 3, 3, True) == [
"#                             ______________ #",
"#                             |              #",
"#                             |              #",
"#               ______________|              #",
"#               |                            #",
"#               |                            #",
"# ______________|                            #",
"# |                                          #",
"# |                                          #",
"# |                                          #"
])
    
def test_document2():
    assert(solve(7, 1, 3, True) == [
"#               _______ #",
"#        _______|       #",
"# _______|              #",
"# |                     #"
])
    
def test_document3():
    assert(solve(5, 1, 4, False) == [
"#                _____ #",
"#           _____      #",
"#      _____           #",
"# _____                #",
"#                      #"
])

