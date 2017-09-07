# astar_search.py
# Kerstin Voigt, Feb 2016, after Nils Nilsson's Graph Search Algorithm
# an expanded version of search.py

# a version of graph_search (from lab5)
import random
#from puzz8 import *
#from jug34 import *
from toh_midterm import *

class Node:
    def __init__(self, num = None, parnum=None, state=None, dep=0, val=None):
        self.no = num # a unique state number
        self.parno = parnum # state number of parent node
        self.thestate = state # the actual state
        self.depth = dep # depth of node in search
        self.eval = val # eval fct value 

    def __eq__(self,other):
        if self.thestate == other.thestate:
            return True
        else:
            return False

# x a "state", nodes a list of elements of type Node

def on_list_of_Nodes(x,lst):
    for y in lst:
        if x == y.thestate:
            return True
    return False

def on_open(x,op):
    return on_list_of_Nodes(x,op)

def on_closed(x,cl):
    return on_list_of_Nodes(x,cl) 


# to sort in the manner of A*; depth of node + eval_fct
def getkey(node):
    return node.depth + node.eval

NUM = 0

# search to GOAL from state start; start could be an 8-puzzle data structure;
def search(start):
    global NUM
    NUM = 1
    all = []
    start_node = Node(NUM,None,start,0,eval_fct(start)) # casting start "node"
    NUM += 1
    # open: a list of all (partial) paths seen until now
    # initial to path to starting point, [x] 
    open = [start_node]
    closed = []
    k = 1
    while open != []:
        if k > 10000:
            break
        curr = open[0]
        open = open[1:]
        closed.append(curr)  # store copy on closed;

        #print "%d. From open... (%d) %s" % (k,curr.no,curr.thestate)
        
        if goal_fct(curr.thestate):
            #return True
            print "\nGOAL reached after expanding %s nodes" % k
            return compute_path(start_node,curr,closed)
        
        else:
            neighs = successor_fct(curr.thestate) # successor "states"
            for n in neighs:
                if not on_open(n,open) and not on_closed(n,closed):
                    new_node = Node(NUM,curr.no,n,\
                                    curr.depth + 1,\
                                    eval_fct(n))
                    open.append(new_node)
                    open.sort(key=getkey)
                    NUM += 1
                elif on_closed(n,closed):
                    # A-STAR: if node n is already on closed and cn is that
                    # that node currently on closed, then, if n's evaluation is
                    # lower than cn's evaluation, reroute cn's parent reference
                    # to C's parent; alternatively, one could replace cn with n
                    # on closed; 
                    for i in range(len(closed)):
                        if n == closed[i].thestate and \
                           curr.depth + 1 + eval_fct(n) < \
                           closed[i].depth + closed[i].eval:
                            closed[i].parno = n.parno
                            break
                else:
                    pass
                    
        k += 1
    return False

# start and stop are of type Node, nodes is a listof Nodes
# returns a list of Nodes on the solution path;

def compute_path(start, stop, nodes):
    if start.thestate == stop.thestate:
        return [start]
    parent = None
    # stop node has a parent with number (.no) equal to stop.parnum
    # find this parent node ...
    # print "tracing back from state %d" % stop.no
    
    for n in nodes:
        if n.no == stop.parno:
            parent = n

    return compute_path(start,parent,nodes) + [stop]

                
# testing for toh (midterm)
toh = TOH()
toh_soln = search(toh)
print '\n'
for i in range(len(toh_soln)):
    print "[Step %d]  %s" % (i+1,toh_soln[i].thestate)
    

    




        
        
        
    




    
          
                                                    
