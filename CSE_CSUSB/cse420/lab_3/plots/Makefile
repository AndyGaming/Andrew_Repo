LIBSDL =  -L/usr/local/lib -Wl,-rpath,/usr/local/lib -lSDL  -lSDL_image  -L./

PROG    = plots
#PROG    = draw_demo
CC=$(CROSS_COMPILE)g++
INCLS	=  -I/usr/include

#source codes
SRCS = $(PROG).cpp draw.cpp  surface.cpp
#substitute .c by .o to obtain object filenames
OBJS = $(SRCS:.cpp=.o)
 
#$< evaluates to the target's dependencies,
#$@ evaluates to the target
 
$(PROG): $(OBJS)
	$(CC) -o $@ $(OBJS)  $(LIBSDL) 
 
$(OBJS):
	$(CC) -c  $*.cpp $(INCLS) 
                                                                                
clean:
	rm $(OBJS)
