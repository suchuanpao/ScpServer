include $(SCP_SCRIPT_PATH)/global_flags.inc

CFLAGS=-I./include
SOURCES=$(shell find -iname *.c)
OBJS=
EXE=

all: $(OBJS)
	@echo -e "$(COMPILER) : $(GLOBAL_DDFLAGS) : $(SOURCES) : $(SCP_SCRIPT_PATH)\n"
	$(COMPILER) $(SOURCES) $(CFLAGS) $() -c

%.o : %.c
	$(COMPILER) $< $(GLOBAL_DDFLAGS) $(CFLAGS) -c $@
