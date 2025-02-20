### Arrows (20 points) ###

def solve(feathers: int, length: int, tips: int) -> list[str]:
    top:str    = '#'
    middle:str = '#'
    bottom:str = '#'

    #Head
    for _ in range(tips):
        top    = ' '+top
        middle = '>'+middle
        bottom = ' '+bottom

    #Body
    for _ in range(length-feathers+1):
        top    = ' '+top
        middle = '='+middle
        bottom = ' '+bottom

    #Body and feather
    for _ in range(feathers-1):
        top    = '\\'+top
        middle = '='+middle
        bottom = '/'+bottom
    
    #Only feather
    top    = '\\'+top
    middle = ' '+middle
    bottom = '/'+bottom

    arrow = [top, middle, bottom]
    print_arrow(arrow)
    return arrow

### helper function to visualize the staircase ###
def print_arrow(arrow: list[str]) -> None:
    for line in arrow:
        print(line)