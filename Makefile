# Makefile shamelessly stolen from ISSSort
.PHONY: clean all

BIN_DIR     := ./bin
SRC_DIR     := ./src
LIB_DIR     := ./lib
INC_DIR     := ./include

ROOTVER     := $(shell root-config --version | head -c1)
ifeq ($(ROOTVER),5)
	ROOTDICT  := rootcint
	DICTEXT   := .h
else
	ROOTDICT  := rootcling
	DICTEXT   := _rdict.pcm
endif

PLATFORM:=$(shell uname)
ifeq ($(PLATFORM),Darwin)
SHAREDSWITCH = -Qunused-arguments -shared -undefined dynamic_lookup -dynamiclib -Wl,-install_name,'@executable_path/../lib/'# NO ENDING SPACE
else
SHAREDSWITCH = -shared -Wl,-soname,# NO ENDING SPACE
endif

ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLDFLAGS  := $(shell root-config --ldflags)
ROOTLIBS     := $(shell root-config --glibs) -lRHTTP -lThread
LIBS         := $(ROOTLIBS)

# Compiler.
CC          = $(shell root-config --cxx)
# Flags for compiler.
CFLAGS		= -c -Wall -Wextra $(ROOTCFLAGS) -g -fPIC
INCLUDES	+= -I$(INC_DIR) -I.

# Linker.
LD          = $(shell root-config --ld)
# Flags for linker.
LDFLAGS 	+= $(ROOTLDFLAGS)

# The object files.
OBJECTS =  		$(SRC_DIR)/AlphaCalibrationGlobals.o \
				$(SRC_DIR)/AlphaSpectrum.o \
				$(SRC_DIR)/AlphaSpectrumFitter.o \
				$(SRC_DIR)/AlphaSpectrumGenerator.o \
				$(SRC_DIR)/AlphaSpectrumManipulator.o \
				$(SRC_DIR)/CalibrationCreator.o \
				$(SRC_DIR)/ChainMaker.o \
				$(SRC_DIR)/CrystalBall.o \
				$(SRC_DIR)/ProgressBar.o
 
# The header files.
DEPENDENCIES =  $(INC_DIR)/AlphaCalibrationGlobals.hh \
				$(INC_DIR)/AlphaSpectrum.hh \
				$(INC_DIR)/AlphaSpectrumFitter.hh \
				$(INC_DIR)/AlphaSpectrumGenerator.hh \
				$(INC_DIR)/AlphaSpectrumManipulator.hh \
				$(INC_DIR)/CalibrationCreator.hh \
				$(INC_DIR)/CrystalBall.hh \
				$(INC_DIR)/ChainMaker.hh \
				$(INC_DIR)/ProgressBar.hh
 
.PHONY : all
all: $(BIN_DIR)/iss_alpha_calibration $(LIB_DIR)/libiss_alpha_calibration.so
 
$(LIB_DIR)/libiss_alpha_calibration.so: iss_alpha_calibration.o $(OBJECTS) iss_alpha_calibrationDict.o
	mkdir -p $(LIB_DIR)
	$(LD) iss_alpha_calibration.o $(OBJECTS) iss_alpha_calibrationDict.o $(SHAREDSWITCH)$@ $(LIBS) -o $@

$(BIN_DIR)/iss_alpha_calibration: iss_alpha_calibration.o $(OBJECTS) iss_alpha_calibrationDict.o
	mkdir -p $(BIN_DIR)
	$(LD) -o $@ $^ $(LDFLAGS) $(LIBS)

iss_alpha_calibration.o: iss_alpha_calibration.cc
	$(CC) $(CFLAGS) $(INCLUDES) $^

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cc $(INC_DIR)/%.hh
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

iss_alpha_calibrationDict.o: iss_alpha_calibrationDict.cc iss_alpha_calibrationDict$(DICTEXT) $(INC_DIR)/RootLinkDef.h
	mkdir -p $(BIN_DIR)
	mkdir -p $(LIB_DIR)
	$(CC) -fPIC $(CFLAGS) $(INCLUDES) -c $<
	cp iss_alpha_calibrationDict$(DICTEXT) $(BIN_DIR)/
	cp iss_alpha_calibrationDict$(DICTEXT) $(LIB_DIR)/

iss_alpha_calibrationDict.cc: $(DEPENDENCIES) $(INC_DIR)/RootLinkDef.h
	$(ROOTDICT) -f $@ -c $(INCLUDES) $(DEPENDENCIES) $(INC_DIR)/RootLinkDef.h


.PHONY : clean cleancode cleanfiles

clean: cleancode cleanfiles

cleancode:
	rm -vf $(BIN_DIR)/iss_alpha_calibration $(SRC_DIR)/*.o $(SRC_DIR)/*~ $(INC_DIR)/*.gch *.o $(BIN_DIR)/*.pcm *.pcm $(BIN_DIR)/*Dict* *Dict* $(LIB_DIR)/*
	
cleanfiles:
	rm -vf alpha*.root images/0[0-9]/*.png images/*.png
		
	
	
	
	
