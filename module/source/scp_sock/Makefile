

DEBUG_OR_RELEASE=$(SCP_DEBUG_OR_RELEASE)
DEFINE_DEBUG_MODE=$(SCP_DEFINE_DEBUG_MODE)

GLOBAL_CFLAGS=$(subst :, ,$(SCP_GLOBAL_CFLAGS))
GLOBAL_LFLAGS=$(subst :, ,$(SCP_GLOBAL_LFLAGS))
GLOBAL_LDFLAGS=$(subst :, ,$(SCP_GLOBAL_LDFLAGS))

CFLAGS=$(subst *,-,$(GLOBAL_CFLAGS))
LFLAGS=$(subst *,-,$(GLOBAL_LFLAGS))
LDFLAGS=$(subst *,-,$(GLOBAL_LDFLAGS))


CC=$(SCP_COMPILER)

MODULE= $(shell basename `pwd`)

SOURCE=$(shell find -iname "*.c")
#SOURCE+=$(shell find -iname "*.cpp")

OBJS=$(patsubst %.c,%.o,$(SOURCE))
#OBJS+=$(patsubst %cpp,%.o,$(SOURCE))

LFLAGS+=
LDFLAGS+=
CFLAGS+=-I$(SCP_INC_PATH)

LIB=lib_scpsock.a

all : $(LIB) $(OBJS)

$(LIB): $(OBJS)
	ar rc $(LIB) $(OBJS)
	mv -f $(OBJS) $(SCP_OPT_PATH)
	mv -f $(LIB) $(SCP_LIB_PATH)

%.o : %.c
	@echo "[ compling $@ ]"
	@$(CC) $< -c $(CFLAGS)

.PHONY : clean
clean:
	rm -f $(SCP_LIB_PATH)/$(LIB) $(SCP_OPT_PATH)/$(OBJS)
	
