# This is the main Makefile of the OwniX operating system.
# It would be more straightforward, if dependencies of the DRIVER 
# would be generated with a separate Makefile in drivers/ dir!

MAIN = #MAIN#
CC = gcc
LD = ld
CCFLAGS = -c -nostdlib -nostdinc -fno-builtin -fno-stack-protector -Iinclude
LDFLAGS = -Tlink.ld
OBJECTS = boot.o layer0.o crt.o superblock.o test.o $(MAIN).o \
 port_io.o fs.o sched.o process.o protect.o printf.o screen.o mem_util.o path_operations.o inode.o string.o  \
 kheap.o system_timer.o phys_mem.o circ_queue.o queue.o iom.o virt_mem.o exec.o system.o 
DRIVERS = driver_table.o driver.o zerodev.o nulldev.o ramdiskdev.o atadev.o timedev.o screendev.o
# FS	= superblock.o

all: $(OBJECTS) $(DRIVERS) $(FS)
	$(LD) $(LDFLAGS) -o kernel $(OBJECTS) $(DRIVERS)

test.o : test/test.c
	$(CC) $(CCFLAGS) test/test.c -o test.o
	
superblock.o : fs/superblock.c
	$(CC) $(CCFLAGS) fs/superblock.c -o superblock.o	

driver.o: drivers/driver.c
	$(CC) $(CCFLAGS) drivers/driver.c -o driver.o
	
driver_table.o: drivers/driver_table.c
	$(CC) $(CCFLAGS) drivers/driver_table.c -o driver_table.o
	
zerodev.o: drivers/zerodev.c
	$(CC) $(CCFLAGS) drivers/zerodev.c -o zerodev.o	
	
nulldev.o: drivers/nulldev.c
	$(CC) $(CCFLAGS) drivers/nulldev.c -o nulldev.o	
	
ramdiskdev.o: drivers/ramdiskdev.c
	$(CC) $(CCFLAGS) drivers/ramdiskdev.c -o ramdiskdev.o
	
atadev.o: drivers/atadev.c
	$(CC) $(CCFLAGS) drivers/atadev.c -o atadev.o
	
clean:
	rm kernel *.o
	
boot.o: boot.s
	nasm -felf boot.s -o boot.o

crt.o : crt.s
	nasm -felf $^ -o $@
	
layer0.o: layer0.s
	nasm -felf $^ -o $@
	
$(MAIN).o : $(MAIN).c
	$(CC) $(CCFLAGS) $^ -o $@

protect.o : protect.c
	$(CC) $(CCFLAGS) $^ -o $@

printf.o : printf.c
	$(CC) $(CCFLAGS) $^ -o $@

screen.o : screen.c
	$(CC) $(CCFLAGS) $^ -o $@
	
port_io.o : port_io.c
	$(CC) $(CCFLAGS) $^ -o $@
	
mem_util.o : mem_util.c
	$(CC) $(CCFLAGS) $^ -o $@
	
path_operations.o : fs/path_operations.c
	$(CC) $(CCFLAGS) $^ -o $@
	
inode.o : fs/inode.c
	$(CC) $(CCFLAGS) $^ -o $@
	
string.o : lib/string.c
	$(CC) $(CCFLAGS) $^ -o $@

timedev.o : drivers/timedev.c
	$(CC) $(CCFLAGS) $^ -o $@
	
file_descriptor.o : fs/file_descriptor.c
	$(CC) $(CCFLAGS) $^ -o $@
	
file_open.o : fs/file_open.c
	$(CC) $(CCFLAGS) $^ -o $@
	
kheap.o : kheap.c
	$(CC) $(CCFLAGS) $^ -o $@
	
system_timer.o : system_timer.c
	$(CC) $(CCFLAGS) $^ -o $@
	
phys_mem.o : mm/phys_mem.c
	$(CC) $(CCFLAGS) $^ -o $@
	
screendev.o : drivers/screendev.c
	$(CC) $(CCFLAGS) $^ -o $@	
	
circ_queue.o : ds/circ_queue.c
	$(CC) $(CCFLAGS) $^ -o $@
	
sched.o : ph/sched.c
	$(CC) $(CCFLAGS) $^ -o $@

process.o : ph/process.c
	$(CC) $(CCFLAGS) $^ -o $@
	
queue.o : ds/queue.c
	$(CC) $(CCFLAGS) $^ -o $@
	
iom.o : iom/iom.c
	$(CC) $(CCFLAGS) $^ -o $@

fs.o : fs/fs.c
	$(CC) $(CCFLAGS) $^ -o $@	

virt_mem.o : mm/virt_mem.c
	$(CC) $(CCFLAGS) $^ -o $@
	
exec.o : exec.c
	$(CC) $(CCFLAGS) $^ -o $@
	
system.o : system.c
	$(CC) $(CCFLAGS) $^ -o $@
	
