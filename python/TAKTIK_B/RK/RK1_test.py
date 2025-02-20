from RK1 import Unit
from RK1 import ragnarok

def is_valid_solution(sol):
    return sol != None

def test_ragnarok1():
    sol = ragnarok([("Odin", "Fenrir"), ("Thor", "Hel"), ("Loki", "Freya")], setup_armies())
    assert(is_valid_solution(sol))

def test_ragnarok2():
    sol = ragnarok([("Fenrir", "Thor"), ("Freya", "Hel"), ("Loki", "Odin")], setup_armies())
    assert(is_valid_solution(sol))

def test_ragnarok2():
    sol = ragnarok([("Freya", "Fenrir"), ("Thor", "Hel"), ("Odin", "Loki")], setup_armies())
    assert(is_valid_solution(sol))

def setup_armies() -> dict[list[Unit]]:
    armies = {}
    odin_army = []
    odin_army.append(Unit("Odin", 25, 20, 1, 100))
    odin_army.append(Unit("Valkyrie 1", 18, 15, 2, 60))
    odin_army.append(Unit("Valkyrie 2", 18, 15, 2, 60))
    odin_army.append(Unit("Einherjar 1", 16, 12, 3, 50))
    odin_army.append(Unit("Einherjar 2", 16, 12, 3, 50))
    odin_army.append(Unit("Huginn", 14, 5, 4, 20))
    odin_army.append(Unit("Muginn", 14, 5, 4, 20))

    thor_army = []
    thor_army.append(Unit("Thor", 30, 18, 1, 110))
    thor_army.append(Unit("Guerrier du Tonnerre 1", 20, 10, 2, 55))
    thor_army.append(Unit("Guerrier du Tonnerre 2", 20, 10, 2, 55))
    thor_army.append(Unit("Guerrier du Tonnerre 3", 20, 10, 2, 55))
    thor_army.append(Unit("Protecteur Asgardien 1", 15, 18, 3, 70))
    thor_army.append(Unit("Protecteur Asgardien 2", 15, 18, 3, 70))

    freya_army = []
    freya_army.append(Unit("Freya", 22, 15, 1, 90))
    freya_army.append(Unit("Archers de Asgard 1", 18, 10, 2, 45))
    freya_army.append(Unit("Archers de Asgard 2", 18, 10, 2, 45))
    freya_army.append(Unit("Archers de Asgard 3", 18, 10, 2, 45))
    freya_army.append(Unit("Felin sacre 1", 20, 8, 3, 40))
    freya_army.append(Unit("Felin sacre 2", 20, 8, 3, 40))

    loki_army = []
    loki_army.append(Unit("Loki", 18, 12, 1, 85))
    loki_army.append(Unit("Illusionniste 1", 15, 10, 2, 40))
    loki_army.append(Unit("Illusionniste 2", 15, 10, 2, 40))
    loki_army.append(Unit("Illusionniste 3", 15, 10, 2, 40))
    loki_army.append(Unit("Serpent de Asgard 1", 25, 6, 3, 35))
    loki_army.append(Unit("Serpent de Asgard 2", 25, 6, 3, 35))
    loki_army.append(Unit("Serpent de Asgard 3", 25, 6, 3, 35))

    hel_army = []
    hel_army.append(Unit("Hel", 24, 16, 1, 95))
    hel_army.append(Unit("Guerrier mort-vivant 1", 16, 12, 2, 55))
    hel_army.append(Unit("Guerrier mort-vivant 2", 16, 12, 2, 55))
    hel_army.append(Unit("Guerrier mort-vivant 3", 16, 12, 2, 55))
    hel_army.append(Unit("Chien des enfers 1", 22, 10, 3, 40))
    hel_army.append(Unit("Chien des enfers 2", 22, 10, 3, 40))

    fenrir_army = []
    fenrir_army.append(Unit("Fenrir", 35, 14, 1, 120))
    fenrir_army.append(Unit("Loup sauvage 1", 20, 10, 2, 45))
    fenrir_army.append(Unit("Loup sauvage 2", 20, 10, 2, 45))
    fenrir_army.append(Unit("Loup sauvage 3", 20, 10, 2, 45))
    fenrir_army.append(Unit("Loup sauvage 4", 20, 10, 2, 45))
    fenrir_army.append(Unit("Loup sauvage 5", 20, 10, 2, 45))
    armies = {"Odin": odin_army,"Thor": thor_army, "Freya": freya_army, "Loki": loki_army, "Hel": hel_army, "Fenrir":fenrir_army}
    return armies