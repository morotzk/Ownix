# gyakorlo make file #

EVERY_C_FILE = $(wildcard *.c)
EVERY_O_FILE = $(wildcard obj\*.o)
COMPILER = gcc
LINKER = ld
COMPILER_FLAGS = -c -nostdlib -nostdinc -fno-builtin -fno-stack-protector -Iinclude
LINKER_FLAGS= -Tlink.ld

linking: $(EVERY_O_FILE)
	$(LD) $(LDFLAGS) -o kernel $(EVERY_O_FILE)

all: $(EVERY_C_FILE)
	echo $(COMPILER) $(COMPILER_FLAGS) $(EVERY_C_FILE) -o $(OBJ_DIR)\$(wildcard *.o)
	$(COMPILER) $(COMPILER_FLAGS) $(EVERY_C_FILE) -o $(OBJ_DIR)\$(wildcard *.o)	
	linking
	
clean:
	echo "clean"
