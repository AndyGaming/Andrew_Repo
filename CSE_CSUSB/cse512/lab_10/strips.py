# Yazhuo Liu
# strips.py
# Lab 10
# Mar 24, 2016

# the type of object that the strips op generator is supposed to return
class STRIPS_OP():
    def __init__(self, p=[], a=[], d=[], n=''):
        self.preconds = p
        self.adds = a
        self.dels = d
        self.name = n
        
    def __str__(self):
        outstr = "%s:\n" % self.name
        outstr += "pres: %s\n" % self.preconds
        outstr += "adds: %s\n" % self.adds
        outstr += "dels: %s\n" % self.dels
        return outstr
    
    def apply(state):
        pass

# generate operator "move x from y to z"
# x is a block; y and z are blocks or floor
# to move x from y to z, preconds are:
# on_y_x ("x is on y")
# clear_x ("x is clear on top")
# clear_z ("z is clear on top")
# after moving x from y to z, the added facts are:
# clear_y
# on_z_x
# facts to delete are:
# on_y_x
# clear_z
def generate_STRIPS_op(x,y,z):
    pres = generate_pres(x,y,z)
    adds = generate_adds(x,y,z)
    dels = generate_dels(x,y,z)
    name = generate_name(x,y,z)
    return STRIPS_OP(pres,adds,dels,name)

def generate_pres(x,y,z):
    p1 = "on_%s_%s" % (y,x)
    p2 = "clear_%s" % x
    p3 = "clear_%s" % z
    return [p1,p2,p3]

def generate_adds(x,y,z):
    a1 = "on_%s_%s" % (z,x)
    a2 = "clear_%s" % y
    return [a1,a2]

def generate_dels(x,y,z):
    d1 = "on_%s_%s" % (y,x)
    d2 = "clear_%s" % z
    return [d1,d2]

def generate_name(x,y,z):
    nm = "move_%s_from_%s_to_%s" % (x,y,z)
    return nm

# generates a dictionary data structure for all moves
# that are possible with the blocks in 'blocks'
def all_ops(blocks):
    all = []
    for b1 in blocks:
        for b2 in blocks + ['floor']:
            for b3 in blocks + ['floor']:
                if b1 != b2 and b1 != b3 and b2 != b3:
                    op = generate_STRIPS_op(b1,b2,b3)
                    all.append(op)
    return all




