# For your imports look at the rulebook to see what is allowed
# Pour les import permis allez vous référer au livre des règlements
"""part_4.py
This is the template file for the part 4 of the Prelim 3.
Ceci est le fichier template pour la partie 4 du Prelim 3.
"""

from itertools import chain, product

# taken from: https://docs.python.org/3/library/itertools.html
def flatten(list_of_lists):
    "Flatten one level of nesting."
    return chain.from_iterable(list_of_lists)


# taken from: https://docs.python.org/3/library/itertools.html
def first_true(iterable, default=False, predicate=None):
    "Returns the first true value or the *default* if there is no true value."
    # first_true([a,b,c], x) → a or b or c or x
    # first_true([a,b], x, f) → a if f(a) else b if f(b) else x
    return next(filter(predicate, iterable), default)


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
    
    # "diagnos-tick" hahaha 
    diagnos_dict = dict(diagnostics)
    # "medic" hehahahaha
    med_dict = dict(medicaments)
    
    meds_side_effects = get_meds_side_effects(prescriptions, med_dict)
    reduced_symptomes = get_reduced_symptomes(symptomes, meds_side_effects)
    
    single_sickness = find_single_sickness(reduced_symptomes, diagnos_dict)
    if single_sickness != None:
        return single_sickness
    many_sicknesses = find_2_sicknesses(reduced_symptomes, diagnos_dict)
    if many_sicknesses != None: 
        return ", ".join(many_sicknesses)
    return 'Maladie inconnue/Diagnostic impossible' 


def get_meds_side_effects(prescriptions: list[str], meds: dict[str, list[str]]):
    # Va chercher les effets secondaires de tous les m/dicaments pris par le patient
    # set() does deduping, because why not
    return set(flatten(meds.get(p, []) for p in prescriptions))


def get_reduced_symptomes(symptomes: list[str], excluded_symptomes: list[str]):
    #Si un symptome n'est pas un effet secondaire
    return list(filter(lambda s: s not in excluded_symptomes, symptomes))


def find_single_sickness(patient_symptoms: list[str], diagnostics: dict[str, list[str]]):
    for dname, dsympt in diagnostics.items():
        if all(psympt in dsympt 
                for psympt 
                in patient_symptoms):
            return dname
    return None


def find_2_sicknesses(patient_symptoms, diagnostics):
    #Si une maladie n'explique pas tous les symptomes, appelle la fonction pour une combinaison de 2 maladies
    for (name1, symptoms1), (name2, symptoms2) in product(**diagnostics):
        all_symptoms = chain(symptoms1, symptoms2)
        if all(psympt in all_symptoms 
               for psympt 
               in patient_symptoms):
            return (name1, name2)
    else:
        return None # we didnt find any pairs that worked for all symptoms