from twoD1 import solve

def test_document1():
    assert(solve(
["#############",
"#    a      #",
"#           #",
"#      b    #",
"#############"
]) == 6.83)
    

def test_document2():
    assert(solve([
"#############",
"#a          #",
"#           #",
"#           #",
"#           #",
"#           #",
"#  b        #",
"#           #",
"#############"]) == 12.39)

     
def test_document3():
    assert(solve([
"########",
"#      #",
"#     b#",
"#      #",
"#      #",
"#    a #",
"#      #",
"########"
]) == 7.16)
