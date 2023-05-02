cmake_minimum_required(VERSION 3.5.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CXX_FLAGS "-Wall -Werror")
set(CMAKE_CXX_FLAGS "${CXX_FLAGS}")

set(SOURCE_FILES src/game.cpp)

project(CPP_GAME)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data/map.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data/)

if(UNIX)
    add_definitions(-std=c++17)
    set(SDL2_LIBRARIES `sdl2-config --cflags --libs` -pthread CACHE STRING "link flags for SDL")
    add_executable(game ${SOURCE_FILES})
    target_link_libraries(game ${SDL2_LIBRARIES})
endif(UNIX)

if(WIN32)
    add_definitions(-std=c++17 -D_USE_MATH_DEFINES -pthread)

    include_directories("C:/msys64/mingw64/include")

    set(MINGW32_LIBRARY mingw32 "-mwindows" CACHE STRING "link flags for MinGW")
    set(SDL2_LIBRARIES SDL2main SDL2 opengl32 CACHE STRING "link flags for SDL")

    string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES)
    
    add_executable(game WIN32 ${SOURCE_FILES})
    target_link_libraries(game ${MINGW32_LIBRARY} ${SDL2_LIBRARIES})

    # MANUAL
    # g++ .\src\game.cpp -IC:\msys64\mingw64\include -lmingw32 -mwindows -lSDL2main -lSDL2 -lopengl32 -pthread

endif(WIN32)
