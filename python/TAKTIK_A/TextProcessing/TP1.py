### Encrypted punctuation (40 points) ###


punctuation = ".,!?"
termination = ".?!"


def find_punct(text):
    punct_idxs = []
    
    for idx, letter in enumerate(text):
        if letter in punctuation:
            punct_idxs.append(idx)
    return punct_idxs

def shifted(text: str, shift: int):
    print()
    punct_idxs= find_punct(text)
    
    shifted_idx = [v for v in punct_idxs]
    
    for _ in range(shift):
        last = shifted_idx.pop(0)
        shifted_idx.append(last)

    newtext = [c for c in text]
    
    for oldidx , newidx in zip(punct_idxs, shifted_idx):
        newtext[newidx] = text[oldidx]
        # newtext = newtext[:newidx] + text[oldidx] + newtext[newidx + 1:]

    print()
    return "".join(newtext)


def is_good(text: str):
    print(text)
    textlist = list(text)
    
    punct_idx = find_punct(text)
    
    lastindex = punct_idx.pop()
    last_punct = textlist[lastindex]
    if not last_punct in termination:
        return False
    
    for idx in punct_idx:
        if textlist[idx] in termination and textlist[idx+2].islower(): # idx+2 bc we want to jump over the space
            return False
            
            
    return True


def solve(text: str) -> int:

    shifts = 0
    
    while (not is_good(shifted(text, shifts))):
        shifts +=1
        
    return shifts

