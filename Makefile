
# The MIT License (MIT)
# 
# Copyright (c) 2025 Tyler Anderson <stolenlan@gmail.com>
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#

MINGW64:=x86_64-w64-mingw32-
MINGW32:=i686-w64-mingw32-

CC:=gcc
WRES:=windres


# CCROSS_FLAGS:=-Wl,--allow-multiple-definition

all: dlse dlse_cli_x86_64.exe dlse_cli_i686.exe dlse_gui_i686.exe dlse_gui_x86_64.exe

# Just a basic binary, to inspect items, not part of dlse
item: item.c
	$(CC) -o $@ item.c

dlse.o: dlse.h dlse.c
	$(CC) -c -o $@ dlse.c

dlse: dlse.o main.c
	$(CC) -o $@ main.c dlse.o

# Windows Resources 64
dlse_x86_64.res: dlse.rc dev/res/app.manifest
	$(MINGW64)$(WRES) -o $@ -O coff dlse.rc

# Windows Resources 32
dlse_i686.res: dlse.rc dev/res/app.manifest
	$(MINGW32)$(WRES) -o $@ -O coff dlse.rc

# Windows Object (dlse) 32
dlse_win_i686.o: dlse.c dlse.h
	 $(MINGW32)$(CC) -c -o $@ dlse.c

# Windwos Object (dlse) 64
dlse_win_x86_64.o: dlse.c dlse.h
	 $(MINGW64)$(CC) -c -o $@ dlse.c

# Windows GUI 64
dlse_gui_x86_64.exe: dlse_gui.c dlse_x86_64.res dlse_win_x86_64.o
	$(MINGW64)$(CC) -o $@ dlse_x86_64.res dlse_win_x86_64.o dlse_gui.c \
		-mwindows -lwinmm -lmsvcrt -lcomctl32 

# Windows GUI 32
dlse_gui_i686.exe: dlse_gui.c dlse_i686.res dlse_win_i686.o
	$(MINGW32)$(CC) -o $@ dlse_gui.c dlse_i686.res dlse_win_i686.o \
		-mwindows -lwinmm -lmsvcrt -lcomctl32 

# Windwos CLI 32
dlse_cli_i686.exe: dlse_win_i686.o dlse_i686.res
	$(MINGW32)$(CC) -o $@ dlse_i686.res dlse_win_i686.o main.c

# Windows CLI 64
dlse_cli_x86_64.exe: dlse_win_x86_64.o dlse_x86_64.res
	$(MINGW64)$(CC) -o $@ dlse_x86_64.res dlse_win_x86_64.o main.c

.PHONY: test
test: dlse
	valgrind --leak-check=full --show-leak-kinds=all ./dlse dev/test.sav

clean:
	rm -f dlse item
	rm -f dlse_cli_x86_64.exe dlse_cli_i686.exe \
		dlse_gui_i686.exe dlse_gui_x86_64.exe
	rm -f *.res
	rm -f *.o