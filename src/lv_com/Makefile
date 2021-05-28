SRCS = $(wildcard *.c) 
OBJS = $(SRCS:.c = .o) 
CC = gcc 
INCLUDES = -I ../../include/
LIBS = -lpthread  -ltool_cups -lcurl
LIBS_PATH= -L ../../lib
#CCFLAGS = -g3 -Wall   -gdwarf-2 -fomit-frame-pointer -pg
CCFLAGS = -g3 -Wall   -gdwarf-2 -O2

../../bin/lv_com : $(OBJS)
	$(CC) $^  $(CCFLAGS)  -o $@ $(INCLUDES) $(LIBS) $(LIBS_PATH)
	@cp  -rf ../../bin/lv_com  .
	
%.o : %.c 
	$(CC)  $(CCFLAGS) -c $^  $(INCLUDES)  $(LIBS) $(LIBS_PATH) 

clean: 
	rm  -f ../../bin/lv_com
.PHONY:clean


