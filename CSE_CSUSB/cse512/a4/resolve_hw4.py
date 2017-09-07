# Yazhuo Liu
# resolve_hw4.py
# code from lab7 exercise, adapted to work for hw4

import random
import copy

# list of given clauses, containing the "premises"; list of clauses that
# represent the negated form of the statement that is to be proved;
# each clause is a list of "literals" (like variables, but in positive or
# negative form);
pqr_given = [['notP', 'notQ', 'R'], ['P', 'R'], ['Q', 'R']]
pqr_refute = [['notR']]  # negated form of statement to prove;

# Problem1:
chem_given = [['notMgO','notH2','Mg'],['notMgO','notH2','H2O'],\
              ['notC','notO2','CO2'],\
              ['notCO2','notH2O','H2CO3'],\
              ['MgO'],['H2'],['O2'],['C']]

chem_refute = [['notH2CO3']]

# Problem2:
gov_given = [['TXI', 'notEXR','DCR'],\
             ['notTXI','CCT'],\
             ['notDCR','IRI','EXR'],['notDCR','IRI','notBMM'],\
             ['TXI', 'CCT','notDCR','BMM'],\
             ['notCCT'],\
             ['notIRI','notEXR']] 
             
gov_refute = [['DCR'],['EXR']]
        
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
    lst1 = []
    for x in lst:
       if not x in lst1:
           lst1.append(x)
    return lst1
       
            
# if c3 not contained in cls (no clause in cls is "same"), then
# add c3 to cls and return new cls; if c3 contained, return cls
# unchanged;
def add_new_clause(cls,c3):
    # test whether c3 contains complementary literals;
    if clause_with_compl(c3):
        print "... no need to add"
        return cls
    # remove duplicate literals if there are any
    #c3 = no_dups(c3)
    # do not add if c3 already in cls
    for x in cls:
        if same_clause(x,c3):
            return cls
    # c3 is new and added
    cls.append(c3)
    print " ... added"
    return cls

def comp_len(x,y):
    return len(x)-len(y)

def resolve(given, refute):
    given.extend(refute)
    cls = given
    count = 1
    while count < 10000:
        #count += 1
        clause_added = False
        before = len(cls)
        #cls.sort(cmp=comp_len)
        random.shuffle(cls)
        for i in range(len(cls)-1):
            c1 = cls[i]
            for j in range(i+1,len(cls)):
                c2 = cls[j]
                #print "[%d.] Checking %s and %s for compl literal ..." %\
                #   (count,c1,c2)
                compl = contains_compl(c1,c2)
                if not compl == None:
                    print "[%d.] Resolving %s and %s ..." % (count,c1,c2)
                    count += 1
                    #print "... found compl lits (%s,%s)" % compl
                    c3 = copy.deepcopy(c1)
                    cp2 = copy.deepcopy(c2)
                    c3.extend(cp2)
                    c3.remove(compl[0])
                    c3.remove(compl[1])
                    c3 = no_dups(c3)
                    print "... new clause %s" % c3

                    if c3 == []:
                        return "UNSATISFIABLE :-)"
                    # add c3 to cls if not contained already
                    cls = add_new_clause(cls,c3)
                    #N += 1
        if len(cls) == before:
            # no new resolvents generated;
            return 'No Contradiction: Satisfiable'
    return

        
            
        
                    


            
                
