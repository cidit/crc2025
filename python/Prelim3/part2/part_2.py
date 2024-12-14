# For your imports look at the rulebook to see what is allowed
# Pour les import permis allez vous référer au livre des règlements
"""part_2.py
This is the template file for the part 2 of the Prelim 3.
Ceci est le fichier template pour la partie 2 du Prelim 3.
"""
proba_full = {
    .10: [.02, .00, .00, .00, .00, .00],
    .20: [.07, .02, .00, .00, .00, .00],
    .30: [.14, .07, .04, .02, .01, .00],
    .40: [.23, .14, .09, .06, .04, .03],
    .50: [.33, .24, .17, .13, .10, .08],
    .55: [.39, .29, .23, .18, .14, .11],
    .60: [.45, .35, .29, .24, .20, .17],
    .65: [.51, .42, .35, .30, .26, .21],
    .70: [.57, .51, .43, .38, .34, .30],
    .75: [.64, .57, .51, .46, .42, .39],
    .80: [.71, .65, .60, .55, .52, .49],
    .85: [.78, .73, .69, .65, .62, .60],
    .90: [.85, .83, .79, .76, .74, .72],
    .95: [.92, .91, .89, .88, .87, .85]
}

def part_2(arrivals: int, mu: int, c: int) -> float:
    """
    Computes the cost of custom made top hats
    
    Parameters:
        arrivals int: the width of the opening of the hat
        mu int: the height of the hat
        c int: the width of the brim

    Returns:
        float: temps d'attente en minutes
    """
    wait_time_minutes = 0
    ### You code goes here ###
    ### Votre code va ici ###
    p = get_ratio(arrivals, mu, c)

    lq = get_Lq(proba_full[p][c-2], p)

    wait_time_minutes = get_wait_time(lq, arrivals, mu)
    print("wait " + str(wait_time_minutes))

    return wait_time_minutes


def get_ratio(a:int, u:int, c:int):
    '''
    Détermine le ratio des arrivés/traités
    '''
    return a/(u*c)

def get_Lq(prob_occ, ratio):
    '''
    Trouve le nombre de clients dans le file d'attente
    '''
    lq = prob_occ * (ratio/(1-ratio))
    return round(lq, 2)

def get_wait_time(lq, a:int, u:int):
    '''
    Trouve le temps d'attente moyen
    '''
    print("lq " + str(lq))
    wait = (((a/u)+lq)/a)*60
    return round(wait, 2)