# toh.py
# to be completed for the CSE 512 midterm, Winter 2016
#
# STUDENT NAME: Yazhuo Liu	004194007

#                      *** MIDTERM ***
# COMPLETE THIS MODULE BY REPLACING EACH OCCURRENCE OF 'pass' WITH THE
# PYTHON CODE THAT WILL IMPLEMENT THE DESCRIBED FUNCTIONALITY.

# TEST by loading and RUN-ing file astar_search_midterm.py;

# DO NOT make any changes to file astar_search_midterm.py, or to any
# of this file that is not marked with 'pass'

# SUBMIT: (1) A hardcopy of the completion of this file
#         (2) A hardcopy of the IDLE window demonstrating loading
#             and running of the programc you get;

SIZE = 5 
toh = range(1,SIZE+1)
toh.reverse()

class TOH:
    def __init__(self, a = toh, b = [], c = []):
        self.pegA = a[:]
        self.pegB = b[:]
        self.pegC = c[:]

    def __str__(self):
        return 'A:%s  B:%s  C:%s' %\
               (self.pegA, self.pegB, self.pegC)

    def __eq__(self,other):
        if other.__class__ == TOH:
            return self.pegA == other.pegA and\
                   self.pegB == other.pegB and\
                   self.pegC == other.pegC
        else:
            return  False

    def pegA (self):
        return self.pegA

    def pegB (self):
        return self.pegB

    def pegC (self):
        return self.pegC

    # move disk from pegA to pegB; if this move is not possible
    # in the current configuration, return None;
    # otherwise make a COPY of self with
    # toh = TOH(self.pegA, self.pegB, selfpegC)
    # and execute the move on the toh object;
    # return modified toh;
    def move_A_to_B(self):
        toh = TOH(self.pegA, self.pegB, self.pegC)
        if not toh.pegA or len(toh.pegB)>5:
            return None
        if not toh.pegB:
            toh.pegB.append(toh.pegA[-1])
            toh.pegA = toh.pegA[:-1]
            return toh
        else:
            if toh.pegA[-1] < toh.pegB[-1]:
                toh.pegB.append(toh.pegA[-1])
                toh.pegA = toh.pegA[:-1]
                return toh
        return None

    # move disk from pegA to pegC; ... 
    def move_A_to_C(self):
        toh = TOH(self.pegA, self.pegB, self.pegC)
        if not toh.pegA or len(toh.pegC)>5:
            return None
        if not toh.pegC:
            toh.pegC.append(toh.pegA[-1])
            toh.pegA = toh.pegA[:-1]
            return toh
        else:
            if toh.pegA[-1] < toh.pegC[-1]:
                toh.pegC.append(toh.pegA[-1])
                toh.pegA = toh.pegA[:-1]
                return toh
        return None   

    def move_B_to_A(self):
        toh = TOH(self.pegA, self.pegB, self.pegC)
        if not toh.pegB or len(toh.pegA)>5:
            return None
        if not toh.pegA:
            toh.pegA.append(toh.pegB[-1])
            toh.pegB = toh.pegB[:-1]
            return toh
        else:
            if toh.pegB[-1] < toh.pegA[-1]:
                toh.pegA.append(toh.pegB[-1])
                toh.pegB = toh.pegB[:-1]
                return toh
        return None

    def move_B_to_C(self):
        toh = TOH(self.pegA, self.pegB, self.pegC)
        if not toh.pegB or len(toh.pegC)>5:
            return None
        if not toh.pegC:
            toh.pegC.append(toh.pegB[-1])
            toh.pegB = toh.pegB[:-1]
            return toh
        else:
            if toh.pegB[-1] < toh.pegC[-1]:
                toh.pegC.append(toh.pegB[-1])
                toh.pegB = toh.pegB[:-1]
                return toh
        return None
    
    def move_C_to_A(self):
        toh = TOH(self.pegA, self.pegB, self.pegC)
        if not toh.pegC or len(toh.pegA)>5:
            return None
        if not toh.pegA:
            toh.pegA.append(toh.pegC[-1])
            toh.pegC = toh.pegC[:-1]
            return toh
        else:
            if toh.pegC[-1] < toh.pegA[-1]:
                toh.pegA.append(toh.pegC[-1])
                toh.pegC = toh.pegC[:-1]
                return toh
        return None
    
    def move_C_to_B(self):
        toh = TOH(self.pegA, self.pegB, self.pegC)
        if not toh.pegC or len(toh.pegB)>5:
            return None
        if not toh.pegB:
            toh.pegB.append(toh.pegC[-1])
            toh.pegC = toh.pegC[:-1]
            return toh
        else:
            if toh.pegC[-1] < toh.pegB[-1]:
                toh.pegB.append(toh.pegC[-1])
                toh.pegC = toh.pegC[:-1]
                return toh
        return None

# given toh, compute all possible next states; 
def successor_fct(toh):
    succs = [toh.move_A_to_B(), toh.move_A_to_C(),\
             toh.move_B_to_A(), toh.move_B_to_C(),\
             toh.move_C_to_A(), toh.move_C_to_B()]
    while None in succs:
        succs.remove(None) 
    return succs

# for a TOH object tw, return True if pegA and pegB are empty,
# and the 5-tower [5,4,3,2,1] sits on pegC
def goal_fct(tw):
    if not tw.pegA and not tw.pegB and tw.pegC == toh:
        return True
    return False

# evaluate the distance of a TOH object by computing the difference
# between 5 (also: global SIZE) and the number of disks on pegC
def eval_fct(toh):
    dis = SIZE - len(toh.pegC)
    return dis
    



    



                       
                       
                       


            
        
        
