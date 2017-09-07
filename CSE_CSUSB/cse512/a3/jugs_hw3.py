# Yazhuo Liu
# CSE 512
# jugs34_hw3.py

# The water jug puzzle; inspired by Nilsson p. 137;
# partial code by KV for CSE 512, Winter 2016
# completed code is to be used with graphsearch.py
# goal state; 2 liters in the 4-liter jug, empty 3 liter jug
GOAL = [0,2]
# initial state; 3- and 4-liter jugs are empty
empty34 = [0,0]
# six operations on water jugs;
# fill 3-liter jug; whatever it contains, after filling it contains 3 liters;
# let jugs be a list [_,_] with first component the contents of teh 3-liter
# jug and second component the contents of the 4-liter jug;
def fill_j3(jugs):
    newjugs = jugs[:]
    [i,j] = newjugs
    newjugs = [3,j]

    print "Fill 3-litter jug."
    return newjugs
# fill 4-liter jug; whatever it contains, after filling it contains 4 liters;
def fill_j4(jugs):
    newjugs = jugs[:]
    [i,j] = newjugs
    newjugs = [i,4]

    print "Fill 4-litter jug."
    return newjugs
# dump entire contents of the 3-liter jug;
def dump_j3(jugs):
    newjugs = jugs[:]
    [i,j] = newjugs
    newjugs = [0,j]

    print "Dump 3-litter jug."
    return newjugs
# dump entire contents of the 4-liter jug;
def dump_j4(jugs):
    newjugs = jugs[:]
    [i,j] = newjugs
    newjugs = [i,0]

    print "Dump 4-litter jug."
    return newjugs
# pour as much as possible of the contents of the 3-liter jug into the
# 4-liter jug; the 3-liter jug may or may not be empty after pouring;
# the 4-liter jug may or may not be full after pouring;
def pour_j3j4(jugs):
    newjugs = jugs[:]
    [i,j] = newjugs
    if j<4 and not i==0:
        if i+j <= 4:
            k = i
            j = i+j
            i = 0
        else:
            k = 3-(i-(4-j))
            i = i-(4-j)
            j = 4
        print "Pour %s litters from 3-litter jug to 4-litter jug." % k
    newjugs = [i,j]
    return newjugs
# pour as much as possible of the contents of the 4-liter jug into the
# 3-liter jug; ...
def pour_j4j3(jugs):
    newjugs = jugs[:]
    [i,j] = newjugs
    if i<3 and not j==0:
        if i+j <= 3:
            k = j
            i = i+j
            j = 0
        else:
            k = 4-(j-(3-i))
            j = j-(3-i)
            i = 3
        print "Pour %s litters from 4-litter jug to 3-litter jug." % k
    newjugs = [i,j]
    return newjugs
# all possible next water jug states; up to 6 possible next states depending
# on how many operations (fill, dump, pour, ...) apply;
def successor_fct (jugs):
    succs = [fill_j3(jugs), fill_j4(jugs), dump_j3(jugs), dump_j4(jugs),\
             pour_j3j4(jugs), pour_j4j3(jugs)]
    while None in succs:
        succs.remove(None)
    return succs
# place holder; keep this unless you can come up with a useful and
# *admissible* evaluation function (= one that consistently underestimates
# the true cost from state to goal.
def eval_fct(jugs):
    return 0
def goal_fct(jugs):
    return jugs == GOAL
    
    
