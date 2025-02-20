# Crossing the river (15 points) #
import math 
def solve(Vb: float, Vc: float, L: int) -> str:
    angle = math.asin(Vc/Vb)
    Vp = Vb*math.cos(angle)

    t = L/Vp
    print(t/60)
    minutes:int = math.floor(t/60)
    sec:int = round(t - minutes*60)

    text_min = str(minutes)
    text_sec = str(sec)
    if len(text_min) == 1:
        text_min = "0"+text_min
    if len(text_sec) == 1:
        text_sec = "0"+text_sec

    return text_min+"m"+text_sec+"s"
    
    