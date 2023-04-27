cmake_minimum_required(VERSION 3.5.1)

set(CMAKE_CXX_STANDARD 17)
add_definitions(-std=c++17 -D_USE_MATH_DEFINES)

include_directories("C:/msys64/mingw64/include")

set(CXX_FLAGS "-Wall -Werror")
set(CMAKE_CXX_FLAGS "${CXX_FLAGS}")

set(MINGW32_LIBRARY mingw32 "-mwindows" CACHE STRING "link flags for MinGW")
set(SDL2_LIBRARIES SDL2main SDL2 opengl32 CACHE STRING "link flags for SDL")

string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES)

project(CPP_GAME)
set(SOURCE_FILES src/game.cpp)

add_executable(game WIN32 ${SOURCE_FILES})
target_link_libraries(game ${MINGW32_LIBRARY} ${SDL2_LIBRARIES})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data/map.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data/)

# MANUAL
# g++ .\src\game.cpp -IC:\msys64\mingw64\include -lmingw32 -mwindows -lSDL2main -lSDL2 -lopengl32
