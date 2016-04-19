
CC=$(SCP_COMPILER)

CDFLAGS=$(addprefix -D,$(subst *,-,$(subst :, ,$(SCP_CDFLAGS))))
CFLAGS=$(subst *,-,$(subst :, ,$(SCP_CFLAGS)))
LFLAGS=$(subst *,-,$(subst :, ,$(SCP_LFLAGS)))
LDFLAGS=$(subst *,-,$(subst :, ,$(SCP_LDFLAGS)))
COMPILE_LIB=$(subst *,-,$(subst :, ,$(SCP_COMPILE_LIB)))

MODULE=$(shell basename `pwd`)
SOURCES=$(shell find -iname "*.c")
OBJS=$(notdir $(patsubst %.c,%.o,$(SOURCES)))
LIB=$(addprefix lib,$(addsuffix $(SCP_LIB_MODE),$(MODULE)))
INCLUDE=$(dir $(shell find ./ -iname "*.h"))


LFLAGS+=
LDFLAGS+=
CDFLAGS+=
CFLAGS+=$(addprefix -I,$(INCLUDE))
CFLAGS+=$()

$(strip $(LFLAGS))
$(strip $(LDFLAGS))

#$(strip $(CDFLAGS))

all : $(LIB) $(OBJS)

$(OBJS): $(SOURCES)
	@echo "[ compling $@ ]"
	$(CC) $< -c $(CFLAGS) $(CDFLAGS)

$(LIB): $(OBJS)
	$(COMPILE_LIB) $(LDFLAGS) $(LIB) $(OBJS)
	mv -f $(OBJS) $(SCP_OPT_PATH)
	mv -f $(LIB) $(SCP_LIB_PATH) 


.PHONY : clean
clean:
	rm -f $(SCP_LIB_PATH)/$(LIB)
	rm -f $(SCP_OPT_PATH)/$(OBJS)
	rm -f $(LIB) 
	rm -f $(OBJS)
	
