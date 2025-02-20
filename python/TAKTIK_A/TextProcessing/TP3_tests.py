from TP3 import solve

def test_document1():
    assert(solve('tourtour') == 'rnofrnof')

def test_document2():
    assert(solve("J'ai acheté 62 fleurs chez Bertrand!") == "iquarfreB sehc zrnelt 5g éfehca !a'J")

def test_document3():
    assert(solve("je mange des pommes vertes avec 55 points roses dessus") == "znzzeq zezor zfu!ob 22 ceva zefrev zewwob zeq e6uaw e?")

def test_document4():
    assert(solve('CRC is absolute cinema!') == "iaweu!c efnlozpa z! CRC")

def test_supp():
    assert(solve("Shall I play the kobold paladin I made the next time we start a D&D campaign?") == "ju6!abwac D&D a frafz em ew!f fxeu ehf eqaw I u!qalab qlopok ehf yalb I llahS")
    assert(solve("j'adore intégrer") == "rer6éfu! eroqa'?")
    assert(solve("viva las vegas") == "za6ev zal av!v")



    