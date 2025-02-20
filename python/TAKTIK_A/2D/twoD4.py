### Smoke and mirrors (50 points) ###

def solve(dimensions: tuple[int, int], n_pos: tuple[int, int], mirrors: list[tuple[int, int, int, int]]) -> bool:
    for mirror in mirrors:
        slope = slope(mirror[0], mirror[1], mirror[2], mirror[3])
        perpandicular = -1/slope
        
        mirror_line = lambda x: slope*-mirror[0]+mirror[1]
        narcissus_perp = lambda x: perpandicular*-n_pos[0]+n_pos[1]
        
        intersectionpt = ()
        
        

    return False    
    
def slope(x1, y1, x2, y2):
    x = (y2 - y1) / (x2 - x1)
    return x