


SOURCES=$(shell 'find -iname "*.c"')
OBJS=$()
#include $(SCP_SCRIPT_PATH)/module_compile_config.inc

all:
	@echo "$(SCP_FLAGS)"
	@echo "[ compling $@ ]"
	@echo "$(SCP_SCRIPT_PATH)"
	$(CC) $(SOURCES) -c $(CFLAGS) $(CDFLAGS)
	mv -f $(OBJS) $(SCP_BIN_PATH)

%.o: %.c




#.PHONY : clean
#clean:
#	rm -f $(SCP_LIB_PATH)/$(LIB)
#	rm -f $(SCP_OPT_PATH)/$(OBJS)
#	rm -f $(LIB) 
#	rm -f $(OBJS)
	
