### This is the test file and you are NOT supposed to modify it (unless you want to add more tests) ###
### Ceci est le fichier de test et vous n'êtes PAS supposé le modifier (sauf si vous voulez ajouter plus de tests) ###

# To run the test run the command pytest in the terminal being in the right folder (go see test instructions if needed)
# Pour rouler les tests avec pytest utilisez le terminal dans le bon folder et exécutez la commande pytest (voir le fichier donnée sur les tests)

from part_4 import part_4

diagnostics=[["Diabète", ["Déshydratation", "Gain de poids", "Perte de poids", "Besoin d'uriner", "Fatigue", "Troubles de vision", "Infections", "Picotements/Engourdissements des extrémités"]],
             ["COVID-19", ["Maux de gorge", "Écoulement nasal", "Éternuements", "Toux", "Fièvre", "Difficultés respiratoires", "Frissons", "Fatigue", "Maux de tête", "Douleurs musculaires", "Perte de l'odorat/du goût"]],
             ["Grippe", ["Toux", "Fièvre", "Douleurs musculaires", "Frissons", "Fatigue", "Maux de gorge", "Maux de tête", "Perte d'appétit", "Écoulement nasal", "Congestion nasale"]],
             ["Rhume", ["Toux", "Fatigue", "Éternuements", "Écoulement nasal", "Congestion nasale", "Maux de gorge"]],
             ["Sclérose en plaques", ["Dépression", "Douleurs", "Faiblesse", "Fatigue", "Intolérance à la chaleur", "Troubles de vision", "Troubles d'équilibre"]],
             ["Syphilis", ["Maux de tête", "Étourdissements", "Troubles de vision", "Troubles d'équilibre", "Troubles de coordination", "Démence", "Perte auditive"]],
             ["Gastroentérite", ["Diarrhée", "Douleurs abdominales", "Nausées", "Vomissements", "Fièvre", "Maux de tête", "Douleurs musculaires", "Perte d'appétit"]],
             ["Mononucléose", ["Fièvre", "Frissons", "Fatigue", "Perte d'appétit", "Douleurs musculaires", "Maux de tête", "Difficultés à avaler"]],
             ["Anémie", ["Pâleur", "Essoufflements", "Fatigue", "Palpitations", "Maux de tête", "Étourdissements", "Faiblesse", "Difficultés à se concentrer"]],
             ["Tuberculose", ["Toux", "Douleurs thoraciques", "Faiblesse", "Fatigue", "Perte de poids", "Perte d'appétit", "Frissons", "Fièvre", "Sueurs nocturnes"]],
             ["Maladie coeliaque", ["Douleurs abdominales", "Diarrhée", "Constipation", "Fatigue", "Dépression"]]
            ]

medicaments=[["Fluvoxamine", ["Nausées", "Insomnie", "Somnolence", "Céphalées", "Étourdissements", "Constipation", "Sécheresse de la bouche", "Vomissements"]],
             ["Antibiotiques", ["Diarrhée", "Fièvre", "Douleurs abdominales", "Perte d'appétit", "Déshydratation", "Nausées"]],
             ["Acétaminophène", ["Sécheresse de la bouche", "Maux de tête", "Étourdissements"]],
             ["Ibuprofène", ["Démangeaisons", "Insomnie", "Nausées", "Vomissements", "Douleurs/Crampes d'estomac", "Diarrhée", "Fatigue", "Indigestion", "Constipation", "Ballonnements"]]
            ]

def test_from_problem_description():
    assert part_4(["Fatigue", "Faiblesse", "Toux"], ["Ibuprofène", "Morphine"], diagnostics, medicaments) == "Tuberculose"
    assert part_4(["Pâleur"], [], diagnostics, medicaments) == "Anémie"
    assert part_4(["Troubles de vision", "Étourdissements", "Maux de tête", "Perte de poids"], ["Acétaminophène"], diagnostics, medicaments) == "Diabète"
    
def test_longer_inputs():
    assert part_4(["Douleurs abdominales", "Diarrhée", "Constipation", "Fatigue", "Dépression", "Troubles d'équilibre", "Fièvre", "Somnolence"], ["Antibiotiques", "Fluvoxamine"], diagnostics, medicaments) == "Sclérose en plaques"
    assert part_4(["Éternuements", "Diarrhée", "Nausées", "Maux de gorge", "Congestion nasale", "Fatigue", "Toux"], ["Antibiotiques", "Diphénhydramine"], diagnostics, medicaments) == "Rhume"


