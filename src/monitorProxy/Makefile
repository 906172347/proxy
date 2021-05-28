SRCS = $(wildcard *.c) 
OBJS = $(SRCS:.c = .o) 
CC = gcc 
INCLUDES = 
LIBS = -lcurl  
LIBS_PATH= 
#CCFLAGS = -g3 -Wall   -gdwarf-2 -fomit-frame-pointer -pg
CCFLAGS = -g3 -Wall   -gdwarf-2 -O2

../../bin/monitorProxy : $(OBJS)
	$(CC) $^  $(CCFLAGS)  -o $@ $(INCLUDES) $(LIBS) $(LIBS_PATH)
	cp  -rf ../../bin/monitorProxy  .
	
%.o : %.c 
	$(CC)  $(CCFLAGS) -c $^  $(INCLUDES)  $(LIBS) $(LIBS_PATH) 

clean: 
	rm  -f ../../bin/monitorProxy 
	rm  -f monitorProxy
.PHONY:clean


