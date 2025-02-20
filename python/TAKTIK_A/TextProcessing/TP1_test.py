from TP1 import solve

def test_document1():
    assert(solve("Bizarre comme les virgules changent tout. Qu'est ce qu'on ferait sans elles, Vraiment! je n'en sais rien!") == 1)
    
def test_document2():
    assert(solve("I really believe the earth is flat! Furthermore! the moon is also flat I can assure you,") == 2)

def test_document3():
    assert(solve("I don’t think it was a good decision. Although, with careful consideration, it wasn’t that bad of a choice either.") == 0)

