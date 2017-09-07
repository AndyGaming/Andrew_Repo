# Yazhuo Liu
# tictactoe_lab6a.py
# by Kerstin Voigt, Feb 2016

# It doesn't work at this moment, there is a bug in the minimax
# function that I haven't been able to fix. 

import random

class T3():
    def __init__(self):
        self.ttt = {'a': 0, 'b': 0, 'c':0, 'd': 0,'e': 0, 'f':0,
               'g': 0, 'h': 0, 'i': 0}
        self.row1 = ['a','b','c']
        self.row2 = ['d','e','f']
        self.row3 = ['g','h','i']
        self.col1 = ['a','d','g']
        self.col2 = ['b','e','h']
        self.col3 = ['c','f','i']
        self.dia1 = ['a','e','i']
        self.dia2 = ['c','e','g']

    def reset(self):
        self.ttt = {'a': 0, 'b': 0, 'c':0, 'd': 0,'e': 0, 'f':0,
               'g': 0, 'h': 0, 'i': 0}

    # row, col, or diag values
    def rcd_values(self,rcd):
        return [self.ttt[x] for x in rcd]

    def __str__(self):
        return " %s %s %s" % (self.rcd_values(self.row1),
                                   self.rcd_values(self.row2),
                                   self.rcd_values(self.row3))

    def __setitem__(self,key,item):
        self.ttt[key] = item
    
    # a way to display the board
    def present(self):
        self.present_row(self.row1)
        self.present_row(self.row2)
        self.present_row(self.row3)
        print("\n")


    def present_row(self,row):
        for i in range(3):
            if self.ttt[row[i]] == 0:
                if i < 2:
                    print(row[i]),
                else:
                    print(row[i])
            else:
                if i < 2:
                    print(self.ttt[row[i]]),
                else:
                    print(self.ttt[row[i]])
        

    # prompt for and put X
    def put_X(self):
        self.present()
        #print self.eval_fct()
        while(True):
            s = self.minimax()
            print "Recommended move: %s" % s
            pick = raw_input('Choose place for X: ')
            if pick in self.ttt and self.ttt[s] == 0:
                if pick == s:
                    self.ttt[s] = 'X'
                    print "\nRecommended move made. \n"
                else:
                    self.ttt[s] = 'X'
                    print "\nRecommended move made. Player move ignored.\n"
                break
            else:
                print "Can't do; choose again:\n"

    # minimax reasoning from player X perspective;
    # returns best next move by indicating one location in [a,b,c,...]
    def minimax(self):
        # phase 1 generate all next moves for X, for each new board
        # generate all next board for @; for each set of next boards
        # determine the board with min eval_fct
        nextX = self.next_moves('X')
        minOs = []
        for xmove in nextX:
            nextO = self.next_moves('@')
            minval = 100
            minmove = None
            for omove in nextO:
                if omove.eval_fct() < minval:
                    minval = omove.eval_fct()
                    minmove = omove

            minOs.append(minmove)
        # minOs is a parallel list to nextX;
        # find the element in nextX with maximal value in minOs;
        bestX = None
        bestval = -100
        for i in range(len(nextX)):
            if minOs[i] > bestval:
                bestval = minOs[i]
                bestX = nextX[i]

        # compare bestX.ttt with self.ttt and identify the location in
        # which both differ; this is the place to recommend to the X player
        # as the best move to make next;
        for k in ['a','b','c','d','e','f','g','h','i']:
            if not bestX.ttt[k] == self.ttt[k]:
                return k

        return None # should never get to this point

        #pass

    # return all boards(objects of type T3) that are next options
    # for player with symb
    def next_moves(self,symb):
        moves = []
        for x in ['a','b','c','d','e','f','g','h','i']:
            if self.ttt[x] == 0:
                # can place symb at position x
                next_ttt = T3()
                for k in ['a','b','c','d','e','f','g','h','i']:
                    next_ttt[k] = self.ttt[k]
                next_ttt.ttt[x] = symb
                moves.append(next_ttt)
        return moves

    # possible rows, cols, diags for x - that for @
    def eval_fct(self):
        x = self.poss_row1('X')+self.poss_row2('X')+self.poss_row3('X')+\
            self.poss_col1('X')+self.poss_col2('X')+self.poss_col3('X')+\
            self.poss_dia1('X')+self.poss_dia2('X')

        o = self.poss_row1('@')+self.poss_row2('@')+self.poss_row3('@')+\
            self.poss_col1('@')+self.poss_col2('@')+self.poss_col3('@')+\
            self.poss_dia1('@')+self.poss_dia2('@')

        return x-o

    def poss_rcds(self, symb):
        return poss_row1(self,symb)+poss_row2(self,symb)+poss_row3(self,symb)+\
               poss_col1(self,symb)+poss_col2(self,symb)+poss_col3(self,symb)+\
               poss_dia1(self,symb)+poss_dia2(self,symb)

    #return 1 if row1 is still possible for player with symb; else return 0
    def poss_row1(self,symb):
        vals = self.rcd_values(self.row1)
        if symb == "X" and "@" in vals:
            return 0
        if symb == "@" and "X" in vals:
            return 0
        return 1

    def poss_row2(self,symb):
        vals = self.rcd_values(self.row2)
        if symb == "X" and "@" in vals:
            return 0
        if symb == "@" and "X" in vals:
            return 0
        return 1

    def poss_row3(self,symb):
        vals = self.rcd_values(self.row3)
        if symb == "X" and "@" in vals:
            return 0
        if symb == "@" and "X" in vals:
            return 0
        return 1

    def poss_col1(self,symb):
        vals = self.rcd_values(self.col1)
        if symb == "X" and "@" in vals:
            return 0
        if symb == "@" and "X" in vals:
            return 0
        return 1

    def poss_col2(self,symb):
        vals = self.rcd_values(self.col2)
        if symb == "X" and "@" in vals:
            return 0
        if symb == "@" and "X" in vals:
            return 0
        return 1

    def poss_col3(self,symb):
        vals = self.rcd_values(self.col3)
        if symb == "X" and "@" in vals:
            return 0
        if symb == "@" and "X" in vals:
            return 0
        return 1

    def poss_dia1(self,symb):
        vals = self.rcd_values(self.dia1)
        if symb == "X" and "@" in vals:
            return 0
        if symb == "@" and "X" in vals:
            return 0
        return 1

    def poss_dia2(self,symb):
        vals = self.rcd_values(self.dia2)
        if symb == "X" and "@" in vals:
            return 0
        if symb == "@" and "X" in vals:
            return 0
        return 1 

    # O responds; will grab or block triples; 
    def play_O(self):
        self.present()
        print "Playing an @ ..."
        ko= self.two_in_any('@')
        if ko:
            self.ttt[ko] = '@'  # to win
        else:
            kx = self.two_in_any('X')
            #print "kx: %s" % kx
            if kx:
                self.ttt[kx] = '@' # to block
            else:
                rest=[]
                for k in self.ttt.keys():
                    if self.ttt[k] == 0:
                        rest.append(k)
                pick = random.choice(rest)
                self.ttt[pick] = '@'

    # True if there is a full row of symbol 'symb'    
    def full_row(self,symb):
        rs = list(3*symb)
        return rs==self.rcd_values(self.row1) or\
               rs==self.rcd_values(self.row2) or\
               rs==self.rcd_values(self.row3)

    # True if there is a full col of symbol 'symb'
    def full_col(self,symb):
        rs = list(3*symb)
        return rs==self.rcd_values(self.col1) or\
               rs==self.rcd_values(self.col2) or\
               rs==self.rcd_values(self.col3)

    # True if there is a full diag of symbol 'symb'
    def full_diag(self,symb):
        rs = list(3*symb)
        return rs==self.rcd_values(self.dia1) or\
               rs==self.rcd_values(self.dia2)

    # True if X wins
    def winX(self):
        return self.full_row('X') or\
               self.full_col('X') or\
               self.full_diag('X')

    # True if O wins
    def winO(self):
        return self.full_row('@') or\
               self.full_col('@') or\
               self.full_diag('@')

    # full board;
    def full(self):
        return not 0 in self.rcd_values(self.row1) and\
               not 0 in self.rcd_values(self.row2) and\
               not 0 in self.rcd_values(self.row3)


    # returns key of any row, col, diag with
    # two symbols where key needs to be blocked/set with @
    # returns False if no two symbols in any row,col,diag
    
    def two_in_any(self,symb):
        r1 = self.two_in_row1(symb)
        r2 = self.two_in_row2(symb)
        r3 = self.two_in_row3(symb)
        c1 = self.two_in_col1(symb)
        c2 = self.two_in_col2(symb)
        c3 = self.two_in_col3(symb)
        d1 = self.two_in_dia1(symb)
        d2 = self.two_in_dia2(symb)
        all = [r1,r2,r3,c1,c2,c3,d1,d2]
        while False in all:
            all.remove(False)
            #print"all: %s" % all
        if all == []:
            return False
        return random.choice(all)
        
    # finds row with two symbs; returns key to block with opposite symbol
    def two_in_row1(self, symb):
        vals = self.rcd_values(self.row1)
        if (vals.count(symb) == 2):
            for k in self.row1:
                if self.ttt[k] == 0:
                    return k
            return False
        else:
            return False

    def two_in_row2(self, symb):
        vals = self.rcd_values(self.row2)
        if (vals.count(symb) == 2):
            for k in self.row2:
                if self.ttt[k] == 0:
                    return k
            return False
        else:
            return False
        
    def two_in_row3(self, symb):
        vals = self.rcd_values(self.row3)
        if (vals.count(symb) == 2):
            for k in self.row3:
                if self.ttt[k] == 0:
                    return k
            return False
        else:
            return False

    def two_in_col1(self, symb):
        vals = self.rcd_values(self.col1)
        if (vals.count(symb) == 2):
            for k in self.col1:
                if self.ttt[k] == 0:
                    return k
            return False
        else:
            return False
                
    def two_in_col2(self, symb):
        vals = self.rcd_values(self.col2)
        if (vals.count(symb) == 2):
            for k in self.col2:
                if self.ttt[k] == 0:
                    return k
            return False
        else:
            return False            

    def two_in_col3(self, symb):
        vals = self.rcd_values(self.col3)
        if (vals.count(symb) == 2):
            for k in self.col3:
                if self.ttt[k] == 0:
                    return k
            return False
        else:
            return False
                
    def two_in_dia1(self, symb):
        vals = self.rcd_values(self.dia1)
        if (vals.count(symb) == 2):
            for k in self.dia1:
                if self.ttt[k] == 0:
                    return k
            return False
        else:
            return False
                
    def two_in_dia2(self, symb):
        vals = self.rcd_values(self.dia2)
        if (vals.count(symb) == 2):
            for k in self.dia2:
                if self.ttt[k] == 0:
                    return k
            return False
        else:
            return False


    # the game loop
    def play(self):
        self.reset()
        print "\n\n"
        print "Starting a new game of tictactoe. X begins ...\n"
        while True:
            #print self.minimax()
            self.put_X()
            if self.winX():
                self.present()
                print "X, you win  :-))\n\n"
                break
            if self.full():
                self.present()
                print "It is a tie :-|\n\n"
                break
            
            self.play_O()
            if self.winO():
                self.present()
                print "O wins, you loose  :-((\n\n"
                break
            if self.full():
                self.present()
                print "Its a tie :-|\n\n"
                break

            
# global; set up for testing

mygame=T3()
#game.put_X()
#game.put_X()
#game.put_X()
#game.put_X()
    

    
            

            

    
               
   
                                   
