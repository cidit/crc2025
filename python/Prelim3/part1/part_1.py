# For your imports look at the rulebook to see what is allowed
# Pour les import permis allez vous référer au livre des règlements
"""part_1.py
This is the template file for the part 1 of the Prelim 3.
Ceci est le fichier template pour la partie 1 du Prelim 3.
"""

from itertools import product, accumulate


def part_1(genes: list[str]) -> float:
    """
    Give the probability to herit a rececive disease

    Parameters:
        genes [str]: the genes from the patient and their ancestors

    Returns:
        float: the probability of having the rececive disease with 2 decimal precision
    """
    possibilities = determine_possibilities(genes, 0)
    
    has_recessive = lambda gene: all(map(is_allele_recessive, gene))
    
    recessives_total = [
        has_recessive(gene) for gene in possibilities
        ].count(True)
    
    # multiply by 100 because we want a percentage
    return 100 * recessives_total/len(possibilities)


def determine_possibilities(genes_tree: list[str], node_to_determine: int):
    '''
    determines the all the possible genes of a node in the tree
    '''
    # if we are a defined gene, returns a list with a 
    # single possibility; ourselves.
    if (current := genes_tree[node_to_determine]) != "XX":
        return [current]
    
    # we want to know all the possible genes our parents can have
    (p_a, p_b) = get_parents_idx(node_to_determine)
    p_a_possibilities = determine_possibilities(genes_tree, p_a)
    p_b_possibilities = determine_possibilities(genes_tree, p_b)

    # find all of the possible resulting genes according to the
    # possible genes of our parents
    possibilities = []
    for p_a in p_a_possibilities:
        for p_b in p_b_possibilities:
            pc = possible_children(p_a, p_b)
            possibilities.extend(pc)
    return possibilities

def get_parents_idx(child_idx: int):
    '''
    find the index of the leaves of any node in a 
    flattened perfect btree
    '''
    parent_a = ((child_idx+1)*2)-1
    parent_b = parent_a+1
    return (parent_a, parent_b)

def is_allele_recessive(allele: str):
   return allele == allele.lower()

def possible_children(parent_a: str, parent_b: str):
    '''
    generates the possibilities of a punnett square for two
    parents as a list of genes
    '''
    return ["".join(p) for p in product(parent_a, parent_b)]
