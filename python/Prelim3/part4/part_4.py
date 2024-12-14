# For your imports look at the rulebook to see what is allowed
# Pour les import permis allez vous référer au livre des règlements
"""part_4.py
This is the template file for the part 4 of the Prelim 3.
Ceci est le fichier template pour la partie 4 du Prelim 3.
"""

from itertools import chain

def flatten(list_of_lists):
    "Flatten one level of nesting."
    return chain.from_iterable(list_of_lists)

def part_4(symptomes, prescriptions, diagnostics, medicaments):
    """
    Computes the position to balance a boat with passengers

    Parameters:
        symptomes [str]: The list of symptomes
        prescriptions [str]: The list of prescritpions of the patient
        diagnostics [[str]]: The list of possible diagnostic with their effects
        medicaments [[str]]: The list of prescriptions possible with their side effects
    
    Returns:
        str: The diagnostic for the patient
    """
    diagnostic = WhatsUpDoc(symptomes, prescriptions, diagnostics, medicaments)
    return ", ".join(diagnostic) # if many maladies, make it a list (but that should never happen)



def WhatsUpDoc(symptomes,prescription,diagnostics,medicaments):

    effets_sec=[]
    reduced_sympt=[]
    test=[]
    flat=[]
    maladies=[]

    #Va chercher les effets secondaires de tous les m/dicaments pris par le patient
    for p in prescription:  
        for m in medicaments:
            if p in m:
                effets_sec.append(m[1])

    #Flatten one level of nesting
    flat = list(flatten(effets_sec))

    #Si un symptome n'est pas un effet secondaire
    for s in symptomes:
        if s not in flat:
            reduced_sympt.append(s)

    #Test pour chaque maladie si elle explique tous les symptomes
    for d in diagnostics:
        test=[]
        for rs in reduced_sympt:
            if rs in d[1]:
                test.append('*') #Pour rester dans le theme des arrays; pourrait etre remplacer par un compteur i++
                if len(test)==len(reduced_sympt):
                    maladies.append(d[0])
    if maladies==[]: #Si une maladie n'explique pas tous les symptomes, appelle la fonction pour une combinaison de 2 maladies
        maladies=WhatTheFrickIsUpDoc(reduced_sympt,diagnostics)
    return maladies
    ####################
    #SHOULD BE STR TYPE#
    ####################


def WhatTheFrickIsUpDoc(reduced_sympt,diagnostics):
    maladies=[]
    for df in diagnostics:
        for dl in diagnostics:
            test=[]
            for rs in reduced_sympt:
                if rs in df[1] or rs in dl[1]:
                    test.append('*')
                    if len(test)==len(reduced_sympt):
                        maladies.append([df[0],dl[0]])
    if maladies==[]:
        return 'Maladie inconnue/Diagnostic impossible'
    
    return list(maladies)
    ####################
    #SHOULD BE STR TYPE#
    ####################
