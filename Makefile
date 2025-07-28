#-s --leak-check=full --show-leak-kinds=all
#x86_64-w64-mingw32-gcc -o gui.exe Win.c -mwindows

CC:=gcc
CCROSS:=x86_64-w64-mingw32-gcc
# CCROSS:=i686-w64-mingw32-gcc
CCROSS_FLAGS:=-Wl,--allow-multiple-definition

all: dlse dlse.exe


getopt.o: getopt.h getopt.c
	$(CC) -o $@ -c getopt.c

getopt_win.o: getopt.h getopt.c
	$(CCROSS) -o $@ -c getopt.c $(CCROSS_FLAGS)

dlse: dlse.c dlse.h getopt.o
	$(CC) -o $@ dlse.c getopt.o

dlse.exe: dlse.c dlse.h getopt_win.o
	$(CCROSS) -o $@ getopt_win.o dlse.c $(CCROSS_FLAGS)

test: all
	@echo "Listing test save-file"
	@./dlse -l "test.sav"

clean:
	rm -f dlse dlse.exe
	rm -f *.o