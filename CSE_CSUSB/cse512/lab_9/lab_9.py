# Yazhuo Liu
# lab 9

import copy

DATATABLE = [{'id':1, 'app':1, 'rat': 0, 'inc': 0, 'bal':1, 'ok': 0},\
             {'id':2, 'app':0, 'rat': 0, 'inc': 1, 'bal':0, 'ok': 0},\
             {'id':3, 'app':1, 'rat': 1, 'inc': 0, 'bal':1, 'ok': 1},\
             {'id':4, 'app':0, 'rat': 1, 'inc': 1, 'bal':1, 'ok': 1},\
             {'id':5, 'app':0, 'rat': 1, 'inc': 1, 'bal':0, 'ok': 0},\
             {'id':6, 'app':1, 'rat': 1, 'inc': 1, 'bal':0, 'ok': 1},\
             {'id':7, 'app':1, 'rat': 1, 'inc': 1, 'bal':1, 'ok': 1},\
             {'id':8, 'app':1, 'rat': 0, 'inc': 1, 'bal':0, 'ok': 0},\
             {'id':9, 'app':1, 'rat': 1, 'inc': 0, 'bal':0, 'ok': 0}]

ATTRIBS = ['app', 'rat', 'inc', 'bal']

DEC = 'ok'

def learn_rules():
    dtab = copy.deepcopy(DATATABLE)
    # rules to be learned
    rules = []
    while not all_pos_covered(rules, dtab):
        nextrule = learn_next_rule(dtab)
        if not nextrule in rules:
            rules.append(nextrule)
        else:
            print "No further rules to be learned. "
            break
        
    return rules

# rules represented as a list of form [[a1,a2],[a3,a4,a5],...]
# where ai are attributes from ATTRIBS

def all_pos_covered(rules,dtab):
    for inst in dtab:
        if pos_instance(inst):
            cov = False
            for r in rules:
                if covers_instance(r,inst):
                    cov = True
                    break
            if cov == False:
                return False

    return True

def covers_instance(rule,inst):
    if rule == []:
        return True
    for at in rule:
        if not inst[at] == 1:
            return False
    return True

def pos_instance(inst):
    return inst['ok'] == 1

def covers_neg(rule,dtab):
    if rule == []:
        return True
    for inst in dtab:
        if covers_instance(rule,inst) and inst['ok']==0:
            return True
    return False

def candidate_rules(rule,attrs):
    cands = []
    for a in attrs:
        cr = rule[:]
        cr.append(a)
        cands.append(cr)
    return cands

def learn_next_rule(dtab):
    attrs = ATTRIBS[:]
    rule = []
    while covers_neg(dtab,rule):
        testrules = candidate_rules(rule,attrs)
        maxratio = -100000
        bestrule = None
        for tr in testrules:
            # compute ratio and keep track of largest
            ratio = comp_ratio(tr,dtab)
            if ratio > maxratio:
                bestrule = tr
                maxratio = ratio
            attrs.remove(bestrule[-1])

    return rule
            
def comp_ratio(rule,dtab):
    poscv,allcv = num_pos_all_covered(rule,dtab)
    return float(poscv)/allcv

def num_pos_all_covered(rule,dtab):
    posnum = 0
    allnum = 0
    for inst in dtab:
        if covers_instance(rule,inst):
            allnum += 1
            if inst['ok'] == 1:
                posnum += 1

    return posnum,allnum

   










            
