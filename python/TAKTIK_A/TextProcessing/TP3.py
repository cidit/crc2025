### Literal reflection (25 points) ###


def replace(char: str):
    match char:
        case 'b' : return 'p'
        case 'd' : return 'q'
        case 'f' : return 't'
        case 'g' : return '6'
        case 'i' : return '!'
        case 'j' : return '?'
        case 'm' : return 'w'
        case 'n' : return 'u'
        case 's' : return 'z'
        case '2' : return '5'
        case 'p' : return  'b'
        case 'q' : return  'd'
        case 't' : return  'f'
        case '6' : return  'g'
        case '!' : return  'i'
        case '?' : return  'j'
        case 'w' : return  'm'
        case 'u' : return  'n'
        case 'z' : return  's'
        case '5' : return  '2'
        case _: return char


def solve(text: str) -> str:
    rev = reversed(text)
    return "".join(map(replace, rev))
    