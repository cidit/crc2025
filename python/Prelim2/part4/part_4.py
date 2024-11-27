# For your imports look at the rulebook to see what is allowed
# Pour les import permis allez vous référer au livre des règlements
"""part_4.py
This is the template file for the part 4 of the Prelim 2.
Ceci est le fichier template pour la partie 4 du Prelim 2.
"""

def part_4(weights, positions):
    """
    Computes the position to balance a boat with passengers

    Parameters:
        weights [int]: The weight of all the passengers
        positions [float]: The position of all the passengers
    
    Returns:
        float or str: the position as a float or if it not valid the error string
    """
    error_message = "On CHATvire!"
    position = 0

    gauche:float = 0
    droite:float = 0

    for i in range(len(weights)):
        if positions[i] < 0:
            gauche += weights[i] * positions[i]
        else:
            droite += weights[i] * positions[i]

    diff = gauche + droite
    position = round(-diff)/100.0

    if position < -0.5 or position > 0.5:
        return error_message

    return position
