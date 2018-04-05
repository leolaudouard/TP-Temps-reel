EXECUTABLE :=  main
SKIN       :=  alchemy	

#IF <xenomai instal dir>/bin is in $PATH can be used just
#XENO_CONFIG := xeno-config
XENO_CONFIG := /usr/xenomai/bin/xeno-config

#Ask script xeno-config necessary parameter to compile
#for a given skin (alchemy(native)|posix|vxworks|psos|alchemy|rtdm|smokey)

#according to man xeno-config
CFLAGS  := $(shell $(XENO_CONFIG) --$(SKIN) --cflags)
LDFLAGS := $(shell $(XENO_CONFIG) --$(SKIN) --ldflags) fonctions.cpp global.cpp lib/src/*
CC      := $(shell $(XENO_CONFIG) --cc)

all: $(EXECUTABLE)

#$(EXECUTABLE):  $(EXECUTABLE).c 
%: %.cpp
	g++ -o $@ $< $(CFLAGS) $(LDFLAGS)

clean:
	@rm $(EXECUTABLE)
