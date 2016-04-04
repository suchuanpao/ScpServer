



CC=gcc 

LFLAGS=-lscp_sock

LLFLAGS=$(addprefix -L,$(SCP_LIB_PATH))

LDFLAGS=

CFLAGS=-g
CCFLAGS=$(addprefix -I,$(SCP_INC_PATH))

SOURCE=$(shell ls *.c)
OBJS=$(patsubst %.c,%.o,$(SOURCE))
EXEC=test

all:
	$(CC) $(SOURCE) $(CFLAGS) $(CCFLAGS) $(LLFLAGS) -o $(EXEC) $(LFLAGS)

clean:
	rm -f $(EXEC)
