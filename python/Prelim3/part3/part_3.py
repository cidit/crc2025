# For your imports look at the rulebook to see what is allowed
# Pour les import permis allez vous référer au livre des règlements
"""part_3.py
This is the template file for the part 3 of the Prelim 3.
Ceci est le fichier template pour la partie 3 du Prelim 3.
"""
from math import floor

def part_3(systolic: int, diastolic: int, heart_rate: int):
    """
    Generate display of heart monitor for 4 heartbeats

    Parameters:
        systolic int: The systolic value
        diastolic int: the diastolic value
        heart_rate int: the heart rate

    Returns:
        [str]: The array of strings composing the monitor for 4 heartbeats
    """
    ### You code goes here ###
    ### Votre code va ici ###
    
    
    scc = systolic_charcount(systolic)
    dcc = diastolic_charcount(diastolic)
    bpmcc = bpm_charcount(heart_rate)
    
    top =       " _     " + (" " * bpmcc)
    systolic =  "| |    " + (" " * bpmcc)
    middle =    "| |_   " + ("_" * bpmcc)
    diastolic = "    | |" + (" "* bpmcc)
    bottom =    "    |_|" + (" " * bpmcc)


    monitor = [top *4,
               *[systolic*4 for _ in range(scc-1)],
               middle*4,
               *[diastolic*4 for _ in range(dcc-1)],
               bottom*4
               ]

    for l in monitor:
        print(repr(l))
    return monitor

def systolic_charcount(systolic: int):
    return floor(systolic/20)

def diastolic_charcount(diastolic: int):
    return floor(diastolic/20)

def bpm_charcount(bpm: int):
    return floor((180-bpm)/10)