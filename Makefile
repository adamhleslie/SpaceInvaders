# Generic GCC Makefile
# Adam Leslie, adamhleslie@gmail.com

#################### Compilation:
# Compiler
CC= g++

# Compiler Flags
CC_FLAGS= -Wall

# Path to includes
# -I../some/path -I/some/other/path
INCLUDES= 

# Path to libraries
# -L/path/to/lib -L/some/other/lib/path
LIB_PATH= 

# Libraries used
# -lmylib -lmyOtherLib
LIBS= -lncurses

#################### File Names:
# Executable
EXE= SpaceInvaders.exe

# Source File Type
SRC_EXT= .cpp

# Object File Type
OBJ_EXT= .o

# Source Folder
SRC_FOLDER = source/

# Object Folder
OBJ_FOLDER =

# Source Files
SRC= $(wildcard $(SRC_FOLDER)*$(SRC_EXT))

# Object Files
OBJ= $(addprefix $(OBJ_FOLDER),$(notdir $(SRC:$(SRC_EXT)=$(OBJ_EXT))))

#################### Rules:
##  % = Implicit/Pattern rule
## $@ = Target file name
## $< = The first dependencies file name

# Phony Targets
.PHONY: all clean test

# Dafault make
all: $(EXE)

# Build Executable
$(EXE): $(OBJ)
	$(CC) $(CC_FLAGS) $(INCLUDES) $(LIB_PATH) -o $(EXE) $(OBJ) $(LIBS) 

# Implicit rule for Object files
$(OBJ_FOLDER)%$(OBJ_EXT): $(SRC_FOLDER)%$(SRC_EXT)
	$(CC) -c $(CC_FLAGS) $(CC_FLAGS) -o $@ $<

# Removes all built files
clean:
	@rm -f $(EXE) $(OBJ)

test:
	@echo Source Files: $(SRC)
	@echo Object Files: $(OBJ)
	@echo Executable File: $(EXE)
