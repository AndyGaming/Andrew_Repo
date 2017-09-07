# Yazhuo Liu
# resolve.py
# code for lab7 exercise

import copy

# list of 4 "clauses"
# each clause is a list of "literals" (like variables, but in positive or
# negative form);
CLS = [['notP', 'notQ', 'R'], ['P', 'R'], ['Q', 'R'], ['notR']]
CLS1 = [['notP', 'notQ', 'R'], ['P', 'R'], ['Q', 'R'], ['R']]


# c1 same as c2, if both contain the same elements, but possibly
# in a different order; iterate over one and test whether it is
# present in the other;

def same_clause(c1,c2):
    if not len(c1) == len(c2):
        return False
    for x in c1:
        if not x in c2:
            return False
    return True

# returns complementary literals if they exist in c1,c2; None else;
def contains_compl(c1,c2):
    for x in c1:
        cx = get_compl(x)
        for y in c2:
            if y == cx:
                return (x,cx)
    return None

# return complement of x; notX if x=X, X if x=notX
def get_compl(x):
    if len(x) > 3:
        if x[:3] == 'not':
            return x[3:]
        else:
            return 'not' + x
    return 'not' + x

def clause_with_compl(c3):
    for i in range(len(c3)-1):
        l1 = c3[i]
        for j in range(i,len(c3)):
            l2 = c3[j]
            if l2 == get_compl(l1):
                return True
    return False

# remove all duplicates elements from list
def no_dups(lst):
    s = set(lst)
    return list(s)
            
# if c3 not contained in cls (no clause in cls is "same"), then
# add c3 to cls and return new cls; if c3 contained, return cls
# unchanged;
def add_new_clause(cls,c3):
    # test whether c3 contains complementary literals;
    if clause_with_compl(c3):
        return cls
    # remove duplicate literals if there are any
    c3 = no_dups(c3)
    # do not add if c3 already in cls
    for x in cls:
        if same_clause(x,c3):
            return cls
    # c3 is new and added
    cls.append(c3)
    print " ... added"
    return cls

def resolve(cls):
    # implement algorithm from lab7 instructions ...
    N = 0
    M = N
    count = 0
    while count < 10000:
        count += 1
        for i in range(len(cls)-1):
            c1 = cls[i]
            for j in range(i,len(cls)):
                c2 = cls[j]
                print "checking %s and %s for compl literal ..." % (c1,c2)
                compl = contains_compl(c1,c2)
                if not compl == None:
                    print "Found compl lits (%s,%s)" % compl
                    c3 = copy.deepcopy(c1)
                    cp2 = copy.deepcopy(c2)
                    c3.extend(cp2)
                    c3.remove(compl[0])
                    c3.remove(compl[1])
                    print "new clause %s" % c3

                    if c3 == []:
                        return "UNSATISFIABLE :-)"
                    # add c3 to cls if not contained already
                    cls = add_new_clause(cls,c3)
                    N += 1
        if N > M:
            M = N
        else:
            ['R', 'R', 'R'] # N has not changed; no new resolvents
            return 'No Contradiction: Satisfiable'
    return

        
            
        
                    


            
                
