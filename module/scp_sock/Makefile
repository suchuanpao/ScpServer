
CC=$(SCP_COMPILER)

DEFINE_DEBUG_MODE=$(SCP_DEFINE_DEBUG_MODE)


DEFINE_FLAGS=$(addprefix -D,$(subst *,-,$(subst :, ,$(SCP_DEFINE_DEBUG_MODE))))
CFLAGS=$(subst *,-,$(subst :, ,$(SCP_CFLAGS)))                     
LFLAGS=$(subst *,-,$(subst :, ,$(SCP_LFLAGS)))                     
LDFLAGS=$(subst *,-,$(subst :, ,$(SCP_LDFLAGS)))                   
COMPILE_LIB=$(subst *,-,$(subst :, ,$(SCP_COMPILE_LIB)))           
 
MODULE=$(shell basename `pwd`)                                     
SOURCE=$(shell find -iname "*.c")                                  
INCLUDE=./include                                                  
OBJS+=$(patsubst %.c,%.o,$(SOURCE))                                
#SOURCE+=$(shell find -iname "*.cpp")                              

LFLAGS+=                                                           
LDFLAGS+=                                                          
CFLAGS+=$(addprefix -I,$(INCLUDE))                                 
DEFINE_FLAGS+=    
LIB=$(addprefix lib,$(addsuffix $(SCP_LIB_MODE),$(MODULE)))        

all : $(LIB) $(OBJS)

$(LIB): $(OBJS)
	ar rc $(LIB) $(OBJS)
	mv -f $(OBJS) $(SCP_OPT_PATH)/$(MODULE)
	mv -f $(LIB) $(SCP_LIB_PATH)

%.o : %.c
	@echo "[ compling $@ ]"
	@echo "makefile MODULE : $(MODULE)"
	@$(CC) $< -c $(CFLAGS)

.PHONY : clean
clean:
	rm -f $(SCP_LIB_PATH)/$(MODULE)/$(LIB) $(SCP_OPT_PATH)/$(MODULE)/$(OBJS)
	
