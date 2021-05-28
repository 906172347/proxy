SRCS = $(wildcard *.c) 
OBJS = $(SRCS:.c = .o) 
CC = gcc 
INCLUDES = -I ../../include/
LIBS = -lpthread  -ltool_cups -lcurl
LIBS_PATH= -L ../../lib
#CCFLAGS = -g3 -Wall   -gdwarf-2 -fomit-frame-pointer -pg
CCFLAGS = -g3 -Wall   -gdwarf-2 -O2

../../bin/proxy : $(OBJS)
	$(CC) $^  $(CCFLAGS)  -o $@ $(INCLUDES) $(LIBS) $(LIBS_PATH)
	cp  -rf ../../bin/proxy  .
	
%.o : %.c 
	$(CC)  $(CCFLAGS) -c $^  $(INCLUDES)  $(LIBS) $(LIBS_PATH) 

clean: 
	rm  -f ../../bin/proxy 
	rm  -f proxy
.PHONY:clean


