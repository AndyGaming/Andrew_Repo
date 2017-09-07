# Yazhuo Liu
# cnf_convert.py
# CNF converstion for propositional logic
# March 2016

p1 = ['notP', '=>', ['R','=>', 'notS']]
p2 = ['not', ['notP', 'and', ['R','=>', 'notS']]]
p3 = [['P','and','R'],'or','S']

def is_literal (x):
    return type(x) == str

def is_neglit(x):
    return is_literal(x) and x[:3] == 'not'

def is_and(x):
    return type(x) == list and len(x) == 3 and x[1] == 'and'

def is_or(x):
    return type(x) == list and len(x) == 3 and x[1] == 'or'

def is_imp(x):
    return type(x) == list and len(x) == 3 and x[1] == '=>'

def is_negex(x):
    return type(x) == list and len(x) == 2 and x[0] == 'not'

# step 1 of cnf converstion: eliminate all implications;
# A => B ---> notA or B

def imp_elim(x):
    if is_literal(x):
        return x
    elif is_or(x):
        return [imp_elim(x[0]), 'or', imp_elim(x[2])]
    elif is_and(x):
        return [imp_elim(x[0]), 'and', imp_elim(x[2])]
    elif is_negex(x):
        return ['not', imp_elim(x[1])]
    else:
        return [['not', imp_elim(x[0])], 'or', imp_elim(x[2])]

def negs_in(x):
    if is_literal(x):
        return x
    elif is_or(x):
        return [negs_in(x[0]), 'or', negs_in(x[2])]
    elif is_and(x):
        return [negs_in(x[0]),  'and', negs_in(x[2])]
    elif is_negex(x):
        return negs_in_negex(x)

def negs_in_negex(x):
    if is_literal(x[1]):
        if is_neglit(x[1]):
            return x[1][3:]
        else:
            return 'not'+x[1]
    elif is_negex(x[1]):
        return negs_in(x[1][1])
    elif is_and(x[1]):
        left = x[1][0]
        right = x[1][2]
        return negs_in([['not', negs_in(left)], 'or', ['not', negs_in(right)]])
    elif is_or(x[1]):
        left = x[1][0]
        right = x[1][2]
        return negs_in([['not', negs_in(left)], 'and', ['not', negs_in(right)]])
    else:
        #should not here, but if, return unchanged
        return x
    
def distrib_andor(x):
    # case 1: x is an expression without 'and'
    if is_literal(x):
        return x
    # case 2: x is a conjunction (that may contain and-or's)
    elif is_and(x):
        return [distrib_andor(x[0]), 'or', distrib_andor(x[2])]
    # case 3: x is a disjunction (that may contain or produce and-or's)
    elif is_or(x):
        left = x[0][0]
        right = x[0][2]
        return [[distrib_andor(left),'or',distrib_andor(x[2])],'and',\
                [distrib_andor(right),'or',distrib_andor(x[2])]]
    else:
        return x





