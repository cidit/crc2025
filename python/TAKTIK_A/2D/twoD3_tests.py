from twoD3 import solve

def test_document1():
    assert(solve(3, 11, 2) == [
'\\\\\\           #',
' ===========>>#',
'///           #'
]
)
    
def test_document2():
    assert(solve(2, 10, 1) == [
'\\\\          #',
' ==========>#',
'//          #']
)
    
def test_document3():
    assert(solve(2, 12, 2) == [
'\\\\             #',
' ============>>#',
'//             #'
]
)