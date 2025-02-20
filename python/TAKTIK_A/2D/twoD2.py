### 2D2 Staircase (65 points) ###
'''
============================================================================================== FAILURES =============================================================================================== 
___________________________________________________________________________________________ test_document1 ____________________________________________________________________________________________

    def test_document1():
>       assert(solve(14, 3, 3, True) == [
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
E       AssertionError: assert [] == ['#          ...      #', ...]
E
E         Right contains 10 more items, first extra item: '#                             ______________ #'
E         Use -v to get more diff

2D\twoD2_tests.py:4: AssertionError
---------------------------------------------------------------------------------------- Captured stdout call ----------------------------------------------------------------------------------------- 
                                          ______________
                                          |
                                          |
                            ______________
                            |
                            |
              ______________
              |
              |
___________________________________________________________________________________________ test_document2 ____________________________________________________________________________________________ 

    def test_document2():
>       assert(solve(7, 1, 3, True) == [
    "#               _______ #",
    "#        _______|       #",
    "# _______|              #",
    "# |                     #"
    ])
E       AssertionError: assert [] == ['#          ...           #']
E
E         Right contains 4 more items, first extra item: '#               _______ #'
E         Use -v to get more diff

2D\twoD2_tests.py:18: AssertionError
---------------------------------------------------------------------------------------- Captured stdout call ----------------------------------------------------------------------------------------- 
                     _______
              _______
       _______
___________________________________________________________________________________________ test_document3 ____________________________________________________________________________________________ 

    def test_document3():
>       assert(solve(5, 1, 4, False) == [
    "#                _____ #",
    "#           _____      #",
    "#      _____           #",
    "# _____                #",
    "#                      #"
    ])
E       AssertionError: assert [] == ['#          ...           #']
E
E         Right contains 5 more items, first extra item: '#                _____ #'
E         Use -v to get more diff

2D\twoD2_tests.py:26: AssertionError
---------------------------------------------------------------------------------------- Captured stdout call ----------------------------------------------------------------------------------------- 
                    _____
               _____
          _____
     _____
======================================================================================= short test summary info ======================================================================================= 
FAILED 2D/twoD2_tests.py::test_document1 - AssertionError: assert [] == ['#          ...      #', ...]
FAILED 2D/twoD2_tests.py::test_document2 - AssertionError: assert [] == ['#          ...           #']
FAILED 2D/twoD2_tests.py::test_document3 - AssertionError: assert [] == ['#          ...           #']'''

def solve(step_l: int, step_h: int, nbr_steps: int, is_full: bool) -> list[str]:
    
    stairs = []
    total_len = step_l *nbr_steps
    for step in range(nbr_steps):
        whitespace_steps = nbr_steps - step -1
        for line in range(step_h):
            toprint = ""
            toprint += " " * (whitespace_steps * step_l)
            if line == 0:
                toprint += "_" * step_l
            elif is_full:
                toprint += "|"
            if is_full and line == 0  and step != 0:
                toprint += "|"
                
                
            
            stairs.append(toprint)
    stairs.append("|" if is_full else "")
    
    def postprocess(line: str):
        padded = line.ljust(total_len)
        return f"# {padded} #"
    
    restairs = list(map(postprocess, stairs))
    
    print_stairs(restairs)
    return restairs



### helper function to visualize the staircase ###
def print_stairs(staircase: list[str]) -> None:
    for line in staircase:
        print(line)
