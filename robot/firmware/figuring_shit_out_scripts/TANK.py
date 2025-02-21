import numpy as np


def set_right(speed: float):
    pass
def set_left(speed: float):
    pass

def tank(x, y):
    # angle = np.arctan2(y, x)
    # # if (y == 0)
    # # !!! x = 0?
    # x_identity = x/abs(x)
    # x = num_to_range(abs(x), 0, 1, -1, 1)
 
    norm = np.linialg.norm([x, y])
    
    if (y == 0):
        set_left(x)
        set_right(-x)
        return
    if (x == 0):
        set_left(y)
        set_right(y)
        return
    if (x > 0 and y > 0):
        # top right
        set_left(norm)
        set_right(y)
    if (x < 0 and y > 0):
        # top left
        set_left(y)
        set_right(norm)
    if (x > 0 and y < 0):
        # bottom right
        set_left(-norm)
        set_right(y) # y is negative
    if (x < 0 and y < 0):
        # bottom left
        set_left(y) # y is negative
        set_right(-norm) 
    
    
    
def num_to_range(num, inMin, inMax, outMin, outMax):
  return outMin + (float(num - inMin) / float(inMax - inMin) * (outMax
                  - outMin))
  

if __name__ == "__main__":
    pass