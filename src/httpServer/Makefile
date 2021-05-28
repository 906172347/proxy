SRCS = $(wildcard *.c) 
OBJS = $(SRCS:.c = .o) 
CC = gcc 
INCLUDES = -I  ../../include
LIBS =  -lpthread
LIBS_PATH= 
#CCFLAGS = -g3 -Wall   -gdwarf-2 -fomit-frame-pointer -pg
CCFLAGS = -g3 -Wall   -gdwarf-2 -O2

../../bin/httSer : $(OBJS)
	$(CC) $^  $(CCFLAGS)  -o $@ $(INCLUDES) $(LIBS) $(LIBS_PATH)
	cp  -rf ../../bin/httSer  .
	
%.o : %.c 
	$(CC)  $(CCFLAGS) -c $^  $(INCLUDES)  $(LIBS) $(LIBS_PATH) 

clean: 
	rm  -f ../../bin/httSer
	rm  -f httSer
.PHONY:clean


