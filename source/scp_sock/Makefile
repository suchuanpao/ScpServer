
CC=$(COMPILER)

MODULE= $(shell basename `pwd`)

CFLAGS=-I$(SCP_INC_PATH)/$(MODULE)

SOURCE=$(shell find -iname "*.c")

OBJS=$(shell find ./ -iname "*.c"|sed -e 's/\.c/\.o/')

LDFLAGS=-O0 -W

LFLAGS=

LIB=lib_scpsock.a



$(LIB):$(OBJS)
	ar rc $(LIB) $(OBJS)

$(OBJS):$(SOURCE)
	$(CC) $< $(CFLAGS) $(LDFLAGS) $(LFLAGS) -c


all:
	mv -f $(OBJS) $(SCP_OPT_PATH)
	mv -f $(LIB) $(SCP_LIB_PATH)

clean:
	rm -f $(SCP_LIB_PATH)/$(LIB) $(SCP_OPT_PATH)/$(OBJS)
	
