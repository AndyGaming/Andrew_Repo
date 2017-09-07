# dotbot0.py

# by Kerstin Voigt, Sept 2014; inspired by Nils Nilsson, Introduction to
# Artificial Intelligence: A New Synthesis

# with modifications Jan 2016

# WITH CLARIFYING COMMENTS FOR THE LECTURE ...

from graphics import *
import random

# global vars
WORLD_MAX_X = 500
WORLD_MAX_Y = 500
GRID = 20
WALL = {}  # introduce GLOBAL VARIABLES and DIRECTORIES

# Wall is a section of contiguous 20x20 black squares on the canvas;
# Wall object builds itself according to user input;

class Wall:
    def __init__(self):
        global WALL   # <<<< global
        prompt = Text(Point(8*GRID, WORLD_MAX_Y - 2*GRID),\
                      "Click one square per click, twice for the last")
        prompt.draw(win)
        prompt_on = True

        click = win.getMouse()
        click1x = click.x - click.x % GRID
        click1y = click.y - click.y % GRID

        while True:
            if prompt_on:
                prompt_on = False
                prompt.undraw()

            # <<<<< why is GRID not declared above like WALL??
            WALL[(click1x,click1y)] = Rectangle(Point(click1x,click1y),\
                                                Point(click1x + GRID,\
                                                      click1y + GRID))
            WALL[(click1x,click1y)].setFill("black")
            WALL[(click1x,click1y)].draw(win)

            click = win.getMouse()
            click2x = click.x - click.x % GRID
            click2y = click.y - click.y % GRID

            # <<< the double-click to stop wall building;
            if (click1x,click1y) == (click2x,click2y):
                break
    
            click1x = click2x
            click1y = click2y

    def draw(self):
        for loc in WALL.keys():
            WALL[loc].draw(win)

    # undrawing is needed for "animation"
    def undraw(self):
        for loc in WALL.keys():
            WALL[loc].undraw()
            


# the dotbot robot ... displayed as a circle; it has location, color, and power;   
class DotBot:
    
    # explain "constructor"
    def __init__(self,loc = Point(5*GRID,5*GRID), col="red", pwr = 100):
        self.location = loc
        self.color = col
        self.the_dotbot = Oval(self.location,\
                               Point(self.location.x + GRID, self.location.y + GRID))
        self.the_dotbot.setFill(self.color)
        self.power = pwr
                               
    # used when object is argument to "print" function; 
    def __str__(self):
        return "%s dotbot at (%d,%d) with power %d" % (self.color,\
                                             self.location.x,\
                                             self.location.y,self.power)
    
    # make sure that .location and  canvas coordinates where dotbot is
    # drawn  match up correctly; 
    def update_dotbot(self):
        self.the_dotbot.move(self.location.x - self.the_dotbot.p1.x,\
                             self.location.y - self.the_dotbot.p1.y)
        
    def draw(self):
        self.update_dotbot()
        self.the_dotbot.draw(win)

    def undraw(self):
        self.the_dotbot.undraw()

    def go(self,where):
        if where == 1:
            self.move_up()
        elif where == 2:
            self.move_down()
        elif where == 3:
            self.move_left()
        elif where == 4:
            self.move_right()
        else:
            pass # a no-op (non-opeation; place holder for instruction
        self.undraw()
        self.draw()
            
    def move_up(self):
        newloc = Point(self.location.x, self.location.y - GRID)
        if self.location.y >= GRID:
            self.location = newloc
      
    def move_down(self):
        newloc = Point(self.location.x, self.location.y + GRID)
        if self.location.y <= WORLD_MAX_Y - GRID:
            self.location = newloc
            
    def move_left(self):
        newloc = Point(self.location.x - GRID, self.location.y)        
        if self.location.x >= GRID:
            self.location = newloc

    def move_right(self):
        newloc = Point(self.location.x + GRID, self.location.y)
        if self.location.x <= WORLD_MAX_X - GRID:
            self.location = newloc

    
# this could be a main function but doesn't have to be ...
# these lines will be executed as part of loading this file ...

win = GraphWin("Dotbot World", WORLD_MAX_X, WORLD_MAX_Y)

# define and display a grid
for i in range(GRID,WORLD_MAX_Y,GRID):
    hline = Line(Point(0,i),Point(WORLD_MAX_X,i))
    hline.draw(win)
    vline = Line(Point(i,0),Point(i,WORLD_MAX_Y))
    vline.draw(win)

# declare and display the dotbot
mybot = DotBot()
mybot.draw()

# declare/build wall space
mywall = Wall()

# interact with user
start = Text(Point(8*GRID, WORLD_MAX_Y-2*GRID), "Click to start the action -- twice to stop")
start.draw(win)

click = win.getMouse()
clickx1 = click.x - click.x % GRID
clicky1 = click.y - click.y % GRID
print "click at %d,%d" % (clickx1, clicky1)

# remove the prompt
start.undraw()

# repeat until double-click ... 
while True:

    # go 1-10 steps into in on a randomly chosen direction;
    for i in range(random.randint(1,10)):
        direct = random.randint(1,4)
        mybot.go(direct)
    
    click = win.getMouse()
    clickx2 = click.x - click.x % GRID
    clicky2 = click.y - click.y % GRID

    print "click at %d,%d" % (clickx2, clicky2)

    if clickx1 == clickx2 and clicky1 == clicky2:
        #clicked same square twice --> quit
        enough = Text(Point(clickx1, clicky1), "Enough of that!")
        enough.draw(win)
        break

    print "bot moved ..."

    clickx1 = clickx2
    clicky1 = clicky2

# two clicks to close ...
win.getMouse()
win.getMouse()
win.close()
    
    

    
            
