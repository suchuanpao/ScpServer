



CC=gcc 

LFLAGS=-lscp_sock

LLFLAGS=$(addprefix -L,$(SCP_LIB_PATH))

LDFLAGS=

CFLAGS=-g -Wall -O0
CCFLAGS=$(addprefix -I,$(SCP_INC_PATH))

SOURCE=server.c
OBJS=$(patsubst %.c,%.o,$(SOURCE))
#INCLUDE=$(dir $(shell find $(SCP_ROOT_PATH) -iname "*.h"))

EXEC=server

all: 
	echo "$(SCP_INC_PATH)"
	$(CC) $(SOURCE) $(CCFLAGS) $(LLFLAGS) $(CFLAGS) -o $(EXEC) $(LFLAGS)


clean:
	rm -f $(EXEC)
