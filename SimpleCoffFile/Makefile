
all: compile64 compile32

all64: compile64 dump64

all32: compile32 dump32

compile64:
	x86_64-w64-mingw32-gcc -c main.c -o main_64.o


compile32:
	i686-w64-mingw32-gcc -c main.c -o main_32.o


dump64:
	x86_64-w64-mingw32-objdump -r main_64.o

dump32:
	i686-w64-mingw32-objdump -r main_32.o

dump: dump64 dump32

clean:
	rm -f main_64.o main_32.o

.PHONY: all compile64 compile32 dump64 dump32 dump clean
