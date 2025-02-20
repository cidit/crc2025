### 2 stacks make a queue (25 points) ###


class fucked_up_stack:
    input_stack: list[int]
    output_stack: list[int]
    
    def __init__(self):
        self.input_stack = []
        self.output_stack = []
        
    def add(self, item: int):
        self.input_stack.append(item)
    
    def rem(self):
        if len(self.output_stack) == 0:
            self.output_stack.extend(reversed(self.input_stack))
            self.input_stack = []
        return self.output_stack.pop()
    

def solve(operations: list[str]) -> list[list[int]]:
    fus = fucked_up_stack()
    
    for op in operations:
        op_code = op[0]
        if op_code == 'A':
            item_to_add = int(op[1:])
            fus.add(item_to_add)
        elif op_code == 'R':
            fus.rem()
    
    return [fus.input_stack, fus.output_stack]

