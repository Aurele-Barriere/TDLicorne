
EXT  = c
CXX  = gcc
EXEC = server client observer

CXXFLAGS = -W -Wall -Wextra -O3
LDFLAGS  = -lSDL2-2.0

SRC_DIR = src
OBJ_DIR = out
SRC = $(notdir $(wildcard  $(SRC_DIR)/*.$(EXT)))
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.$(EXT)=.o))


ASTYLE_OPTIONS =        --style=attach --indent=spaces=4 -A1


ifeq (newclass,$(firstword $(MAKECMDGOALS)))
  CLASS_N := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  $(eval $(CLASS_N):;@:)
  FILE_N := `echo $(CLASS_N) | tr A-Z a-z`
endif

all:  $(EXEC)
	

$(EXEC) : $(OBJ)
	$(CXX) -o $@ $(OBJ_DIR)/$@.o $(filter-out $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(EXEC))), $^) $(LDFLAGS)
	


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.$(EXT) $(wildcard $(SRC_DIR)/%.h) $(SRC_DIR)/defines.h
	@mkdir -p $(OBJ_DIR)
	$(CXX) -o $@ -c $< $(CXXFLAGS)


	
.PHONY: clean mrproper help newclass soupe_aux_choux astyle


soupe_aux_choux :
	@cvlc soupe_aux_choux.mp3
	
newclass :
	@echo "#pragma once\n\n\nclass $(CLASS_N)\n{\n    public:\n        $(CLASS_N)();\n        ~$(CLASS_N)();\n\n    private:\n};" > src/$(FILE_N).h
	@echo "#include \"$(FILE_N).h\"\n\n\n$(CLASS_N)::$(CLASS_N)()\n{\n    \n}\n\n\n$(CLASS_N)::~$(CLASS_N)()\n{\n    \n}\n" > src/$(FILE_N).cpp
	@echo "Created src/$(FILE_N).cpp and src/$(FILE_N).h"

clean :
	@rm -rf $(OBJ_DIR)


mrproper : clean
	@rm -rf $(EXEC)
	
	
astyle:
	astyle $(ASTYLE_OPTIONS) $(wildcard $(SRC_DIR)/*.$(EXT)) $(wildcard $(SRC_DIR)/*.h)
	@rm -rf $(SRC_DIR)/*.orig

help:
	@echo 'Makefile for project `7 colors`'
	@echo ''
	@echo 'Usage :'
	@echo 'make                         Build project executable'
	@echo 'make all                     Build project executable'
	@echo 'make clean                   Clean objects directory'
	@echo 'make mrproper                Clean objects directory and binaries'
	@echo 'make astyle                  Correctly indents source files'
	@echo 'make newclass <class name>   Create a new c++ class'
	@echo 'make help                    Show this help message'
	@echo ''
