from MT4 import solve

def is_valid_solution(grid, sol):
    return sol != None


def test_document1():
    grid = ['########',
'#AA   B#',
'#C  D B#',
'#C@@D B ',
'#C  D  #',
'#E   FF#',
'#E GGG #',
'########'
]
    sol = solve(grid)
    assert(is_valid_solution(grid, sol))


def test_document2():
    grid = ['########',
'#A  BBB#',
'#A  C D#',
'#@@ CED ',
'#FFF ED#',
'#  G HH#',
'#IIGJJ #',
'########'
]
    sol = solve(grid)
    assert(is_valid_solution(grid, sol))

def test_document3():
    grid = ['########',
'#      #',
'#      #',
'# @@A   ',
'# BBA C#',
'# D A C#',
'# DEE C#',
'########'
]
    sol = solve(grid)
    assert(is_valid_solution(grid, sol))