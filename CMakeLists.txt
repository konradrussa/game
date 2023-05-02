cmake_minimum_required(VERSION 3.5.1)

project(CPP_GAME)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CXX_FLAGS "-Wall -Werror")
set(CMAKE_CXX_FLAGS "${CXX_FLAGS}")

set(SOURCE_FILES src/game.cpp)
add_definitions(-std=c++17)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data/map.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data/)

if(UNIX)
    #start
    # 1 Use for readme build
    # set(SDL2_LIBRARIES `sdl2-config --cflags --libs` CACHE STRING "link flags for SDL")
    # end 1
    # 2 use for older setup
    find_package(SDL2 REQUIRED)
    string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES)
    #end 2
    add_executable(game ${SOURCE_FILES})
    target_link_libraries(game ${SDL2_LIBRARIES} -pthread)
endif(UNIX)

if(WIN32)
    include_directories("C:/msys64/mingw64/include")

    set(MINGW32_LIBRARY mingw32 "-mwindows" CACHE STRING "link flags for MinGW")
    set(SDL2_LIBRARIES SDL2main SDL2 opengl32 CACHE STRING "link flags for SDL")

    string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES)
    
    add_executable(game WIN32 ${SOURCE_FILES})
    target_link_libraries(game ${MINGW32_LIBRARY} ${SDL2_LIBRARIES} -pthread)

    # MANUAL
    # g++ .\src\game.cpp -IC:\msys64\mingw64\include -lmingw32 -mwindows -lSDL2main -lSDL2 -lopengl32 -pthread

endif(WIN32)
