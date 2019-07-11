#setup contral
#
#CXX     = g++
#LD      = g++
#LDFLAGS = -m64 

TOP     := $(shell pwd)/
BIN     := $(TOP)bin/
SRC     := $(TOP)src/
INCLUDE := $(TOP)include/
LIB     := $(TOP)lib/

CXX       := $(shell root-config --cxx)
CXX_FLAGS := $(shell root-config --cflags)
CXX_FLAGS += -Wall -Wextra -ggdb
LIBRARIES := $(shell root-config --glibs)


CORR2D_D0  := corr2D_trg_d0 
CORR2D_REF := corr2D_trg_ref


all: $(BIN)/$(CORR2D_D0) $(BIN)/$(CORR2D_REF)

$(BIN)/$(CORR2D_D0): $(SRC)/corr2D_trg_d0.cc $(SRC)/*.cxx
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES)

$(BIN)/$(CORR2D_REF): $(SRC)/corr2D_trg_ref.cc $(SRC)/*.cxx
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES)

clean:
	-rm $(BIN)/*

