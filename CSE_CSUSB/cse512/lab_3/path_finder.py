# Yazhuo Liu
# CSE 512 Assignment 2
# 2/9/16
# path_finder.py
# originally by Kerstin Voigt, Jan 2016; for CSE 512 lecture/lab

# a directed graph
TheMap = {'a':['b','c'], 'b':['a','d','g'], 'c':['a','q'], 'd':['b','h','e'],\
          'e':['f','d'], 'f':['e','i'], 'g':['b','h'], 'h':['g','i','d','m'],\
          'i':['h','f','j','o'], 'j':['i','k','p'], 'k':['j'], 'l':['m'],\
          'm':['l','h','n'], 'n':['m'], 'o':['i','p'], 'p':['o','j','q'], 'q':['c','p']}

# a "search" node with identifying label and parent node;
class Node:
    def __init__(self, par=None, lab=None):
        self.parent = par
        self.label = lab

def next(x):
    return TheMap[x]

# x = a label, nodes = a list of elements of type Node
def on_list_of_Nodes(x,lst):
    for y in lst:
        if x == y.label:
            return True
    return False

def on_open(x,op):
    return on_list_of_Nodes(x,op)

def on_closed(x,cl):
    return on_list_of_Nodes(x,cl)

def search(x,y,sw=0):
    # open: a list of all (partial) paths seen until now
    # initialize to path to starting point, [x]
    open = [Node(None,x)]
    closed = []
    # init counter
    counter = 1
    while open != []:
        # break if after 10000 iterations
        if counter > 10000:
            break
        
        curr = open[0]
        open = open[1:]
        closed.append(curr)
        # print out next node, with iteration num
        print "[%d] Next node from open: %s\n" % (counter, curr.label)
        
        if curr.label == y:
            #return True
            print "The path from '%s' to '%s': " % (x,y)
            return compute_path(x,curr,closed)
        else:
            neighs = next(curr.label)
            for n in neighs:
                if not on_open(n,open) and not on_closed(n,closed):
                    # sw = 1: add to front of open
                    # otherwise add to back
                    if sw == 1:
                        open = [Node(curr,n)] + open
                    else:
                        open.append(Node(curr,n))
        # update counter
        counter += 1

    return False

# start = label of the start node, end = label of the end node;
# nodes = a list of elements of type Node
def compute_path(start,end,nodes):
    path = [end.label]
    # keep looking at the parent node
    while end.label != start:
        for x in nodes:
            if x.label == end.parent.label:
                end = x
                path = [end.label] + path
                break

    return path



        

