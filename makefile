build:	bin/main.exe

run: bin/main.exe
	bin/main.exe

bin/main.exe:	main.cpp	bin/fileread.o	bin/types.o
	g++.exe \
	-Wall \
	-I. \
	-static-libstdc++ \
	-mwindows \
	-g \
	main.cpp \
	-o \
	bin/main.exe \
	"C:/Users/Ersa/Desktop/code/c++/trueopengl/bin/fileread.o" \
	"C:/Users/Ersa/Desktop/code/c++/trueopengl/bin/types.o" \
	-LC:/msys64/mingw64/lib \
	-lglew32 \
	-lfreetype \
	-L"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22000.0/um/x64" \
	-lGdi32 \
	-lOpenGL32 \
	-lpthread \

bin/fileread.o:	lib/fileread.cpp
	g++.exe \
	-Wall \
	-mwindows \
	-c \
	-g \
	lib/fileread.cpp \
	-o \
	bin/fileread.o

bin/types.o: lib/types.cpp
	g++.exe \
	-fimplicit-templates \
	-Wall \
	-mwindows \
	-c \
	-g \
	lib/types.cpp \
	-o \
	bin/types.o

objects:	bin/types.o	bin/fileread.o