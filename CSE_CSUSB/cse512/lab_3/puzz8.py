# Yazhuo Liu
# CSE 512 Lab 3
# puzz8.py
# 2/4/16

import random
import copy

GOAL = {(1,1):1, (1,2):2, (1,3): 3,
        (2,1):8, (2,2):'B', (2,3):4,
        (3,1):7, (3,2):6, (3,3):5}

def make_puzz8():
    #[x1,x2,x3,y1,y2,y3,z1,z2,z3] = range (1,9) + ['B']
    tiles = range(1,9)
    tiles.append('B')
    random.shuffle(tiles)
    puzz8 = {}
    k = 0
    for i in range(1,4):
        for j in range(1,4):
            puzz8[(i,j)] = tiles[k]
            k+=1
    return puzz8

def show_puzz8(pz):
    if pz == None:
        return
    print "\n"
    for i in range(1,4):
        for j in range(1,4):
            print "%s " % pz[(i,j)],
        print "\n"
    return

def goal_fct(pz):
    score = 0
    # compare each element in pz and GOAL
    for (i,j) in pz:
        if pz[(i,j)]==GOAL[(i,j)]:
            score += 1
    return score
   
def succssor_fct(pz):
    moves = [move_blank_up(pz),\
             move_blank_down(pz),\
             move_blank_left(pz),\
             move_blank_right(pz)]
    succs = []
    for x in moves:
        if not x == None:
            succs.append(x)
        return succs
    
#moving a blank up ... B at (i,j) moves to (i-1, j), tile at (i-1, j)
#moves to (i,j)
def move_blank_up(pz):
    newpz = copy.deepcopy(pz)
    keys = newpz.keys()
    blank = None
    for (i,j) in keys:
        if newpz[(i,j)] == 'B':
            blank = (i,j)
            break
        
    #need to test whether blank can still move up;
    #if not return None
    if blank[0] == 1:
        #blank cannot move up
        return None
    
    tile = newpz[(i-1,j)]
    newpz [(i-1,j)] = 'B'
    newpz[(i,j)] = tile
    return newpz

def move_blank_down(pz):
    newpz = copy.deepcopy(pz)
    keys = newpz.keys()
    blank = None
    for (i,j) in keys:
        if newpz[(i,j)] == 'B':
            blank = (i,j)
            break
        
    #need to test whether blank can still move down;
    #if not return None
    if blank[0] == 3:
        #blank cannot move down
        return None
    
    tile = newpz[(i+1,j)]
    newpz [(i+1,j)] = 'B'
    newpz[(i,j)] = tile
    return newpz

def move_blank_right(pz):
    newpz = copy.deepcopy(pz)
    keys = newpz.keys()
    blank = None
    for (i,j) in keys:
        if newpz[(i,j)] == 'B':
            blank = (i,j)
            break
        
    if blank[1] == 3:
        #blank cannot move down
        return None
    
    tile = newpz[(i,j+1)]
    newpz [(i,j+1)] = 'B'
    newpz[(i,j)] = tile
    return newpz

def move_blank_left(pz):
    newpz = copy.deepcopy(pz)
    keys = newpz.keys()
    blank = None
    for (i,j) in keys:
        if newpz[(i,j)] == 'B':
            blank = (i,j)
            break
        
    if blank[1] == 1:
        #blank cannot move down
        return None
    
    tile = newpz[(i,j-1)]
    newpz [(i,j-1)] = 'B'
    newpz[(i,j)] = tile
    return newpz     
   
