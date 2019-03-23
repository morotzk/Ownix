# This is the main Makefile of the OwniX operating system.
# It would be more straightforward, if dependencies of the DRIVER 
# would be generated with a separate Makefile in drivers/ dir!

MAIN = #MAIN#
CC = gcc
LD = ld
EVERY_C_FILE = $(wildcard *.c)
EVERY_O_FILE = $(wildcard *.o)
OBJ_DIR = obj
CCFLAGS = -c -nostdlib -nostdinc -fno-builtin -fno-stack-protector -Iinclude
LDFLAGS = -Tlink.ld
#OBJECTS = boot.o layer0.o crt.o superblock.o test.o $(MAIN).o \
# port_io.o fs.o sched.o process.o protect.o printf.o screen.o mem_util.o path_operations.o inode.o string.o  \
# kheap.o system_timer.o phys_mem.o circ_queue.o queue.o iom.o virt_mem.o exec.o system.o 
OBJECTS = $(wildcard obj\$(EVERY_O_FILE))

DRIVERS = driver_table.o driver.o zerodev.o nulldev.o ramdiskdev.o atadev.o timedev.o screendev.o
 FS	= superblock.o

all: $(EVERY_C_FILE)
	$(CC) $(CCFLAGS) $(EVERY_C_FILE) -o $(OBJ_DIR)\$(wildcard *.o)
#all: $(OBJECTS) $(DRIVERS) $(FS)
	$(LD) $(LDFLAGS) -o kernel $(OBJECTS) $(DRIVERS)
	echo $(EVERY_C_FILE)

test.o : test/test.c
	$(CC) $(CCFLAGS) test/test.c -o $(OBJ_DIR)\test.o
	
superblock.o : fs/superblock.c
	$(CC) $(CCFLAGS) fs/superblock.c -o $(OBJ_DIR)\superblock.o	

driver.o: drivers/driver.c
	$(CC) $(CCFLAGS) drivers/driver.c -o $(OBJ_DIR)\driver.o
	
driver_table.o: drivers/driver_table.c
	$(CC) $(CCFLAGS) drivers/driver_table.c -o $(OBJ_DIR)\driver_table.o
	
zerodev.o: drivers/zerodev.c
	$(CC) $(CCFLAGS) drivers/zerodev.c -o $(OBJ_DIR)\zerodev.o	
	
nulldev.o: drivers/nulldev.c
	$(CC) $(CCFLAGS) drivers/nulldev.c -o $(OBJ_DIR)\nulldev.o	
	
ramdiskdev.o: drivers/ramdiskdev.c
	$(CC) $(CCFLAGS) drivers/ramdiskdev.c -o $(OBJ_DIR)\ramdiskdev.o
	
atadev.o: drivers/atadev.c
	$(CC) $(CCFLAGS) drivers/atadev.c -o $(OBJ_DIR)\atadev.o
	
clean:
	del /Q obj\$(EVERY_O_FILE)
	
debug:
	echo $(EVERY_C_FILE)
	echo $(EVERY_O_FILE)	
	
boot.o: boot.s
	nasm -felf boot.s -o $(OBJ_DIR)\boot.o

crt.o : crt.s
	nasm -felf $^ -o $(OBJ_DIR)\$@
	
layer0.o: layer0.s
	nasm -felf $^ -o $(OBJ_DIR)\$@
	
$(MAIN).o : $(MAIN).c
	$(CC) $(CCFLAGS) $^ -o $(OBJ_DIR)\$@

protect.o : protect.c
	$(CC) $(CCFLAGS) $^ -o $(OBJ_DIR)\$@

printf.o : printf.c
	$(CC) $(CCFLAGS) $^ -o $(OBJ_DIR)\$@

screen.o : screen.c
	$(CC) $(CCFLAGS) $^ -o $(OBJ_DIR)\$@
	
port_io.o : port_io.c
	$(CC) $(CCFLAGS) $^ -o $(OBJ_DIR)\$@
	
mem_util.o : mem_util.c
	$(CC) $(CCFLAGS) $^ -o $(OBJ_DIR)\$@
	
path_operations.o : fs/path_operations.c
	$(CC) $(CCFLAGS) $^ -o $(OBJ_DIR)\$@
	
inode.o : fs/inode.c
	$(CC) $(CCFLAGS) $^ -o $(OBJ_DIR)\$@
	
string.o : lib/string.c
	$(CC) $(CCFLAGS) $^ -o $(OBJ_DIR)\$@

timedev.o : drivers/timedev.c
	$(CC) $(CCFLAGS) $^ -o $(OBJ_DIR)\$@
	
file_descriptor.o : fs/file_descriptor.c
	$(CC) $(CCFLAGS) $^ -o $(OBJ_DIR)\$@
	
file_open.o : fs/file_open.c
	$(CC) $(CCFLAGS) $^ -o $(OBJ_DIR)\$@
	
kheap.o : kheap.c
	$(CC) $(CCFLAGS) $^ -o $(OBJ_DIR)\$@
	
system_timer.o : system_timer.c
	$(CC) $(CCFLAGS) $^ -o $(OBJ_DIR)\$@
	
phys_mem.o : mm/phys_mem.c
	$(CC) $(CCFLAGS) $^ -o $(OBJ_DIR)\$@
	
screendev.o : drivers/screendev.c
	$(CC) $(CCFLAGS) $^ -o $(OBJ_DIR)\$@	
	
circ_queue.o : ds/circ_queue.c
	$(CC) $(CCFLAGS) $^ -o $(OBJ_DIR)\$@
	
sched.o : ph/sched.c
	$(CC) $(CCFLAGS) $^ -o $(OBJ_DIR)\$@

process.o : ph/process.c
	$(CC) $(CCFLAGS) $^ -o $(OBJ_DIR)\$@
	
queue.o : ds/queue.c
	$(CC) $(CCFLAGS) $^ -o $(OBJ_DIR)\$@
	
iom.o : iom/iom.c
	$(CC) $(CCFLAGS) $^ -o $(OBJ_DIR)\$@

fs.o : fs/fs.c
	$(CC) $(CCFLAGS) $^ -o $(OBJ_DIR)\$@	

virt_mem.o : mm/virt_mem.c
	$(CC) $(CCFLAGS) $^ -o $(OBJ_DIR)\$@
	
exec.o : exec.c
	$(CC) $(CCFLAGS) $^ -o $(OBJ_DIR)\$@
	
system.o : system.c
	$(CC) $(CCFLAGS) $^ -o $(OBJ_DIR)\$@
	
