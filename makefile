build:	main.exe
	

main.exe:	main.cpp
	C:\\msys64\\mingw64\\bin\\g++.exe\
    -fdiagnostics-color=always\
    -static-libstdc++\
    -mwindows\
    -g\
    main.cpp\
    -o\
    main.exe\
    -LC:\\msys64\\mingw64\\lib\
    -lglew32\
    -lfreetype\
    -L"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22000.0/um/x64"\
    -lGdi32\
    -lOpenGL32\
    -lpthread\