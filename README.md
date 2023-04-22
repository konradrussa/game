
Windows:

Download and install MSYS2, add bin to System Path

Download SDL, compile with Cmake GUI, from build/include-config copy SDL_config.h file to build/include, copy build/include to your project

Manual compilation:

g++ .\game.cpp -ID:\C++\repo\game\include -lmingw32 -mwindows -lSDL2main -lSDL2 -lopengl32
