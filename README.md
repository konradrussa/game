# Introduction

This is capstone project based on Curriculum - Udacity C++ Nanodegree.

It's game released with SDL library.

## Project structure contains

map file:

* **data/map.txt**

compilation files:

* **game.cpp**

and include header files:

* **game.h** - class Game for main loop and resources initialization

* **map.h** - class GameMap to read map text file and create states vectors

* **interation.h** - class GameInteraction for user input interaction

* **player.h** - class Sprite, astract Movable, class Player, class Enemy

* **controller.h** - class GameController it takes player location and send it to to enemy

* **renderer.h** - class GameRenderer, responsible fro maintaining SDL window, renderer and render screen

* **astar.h** - class with additional enums and A* algorithm implementation (will be used in near future to improve Enemy action)

## Additional information based on requirements

### Compiling and Testing

The project code must compile and run without errors. We strongly recommend using cmake and make, as provided in the starter repos. If you choose another build system, the code must compile on any reviewer platform.
***(based on cmake)***

### Loops, Functions, I/O

A variety of control structures are used in the project. 
***(it's common to the project)***

The project code is clearly organized into functions. 
***(it's common to the project)***

The project reads data from an external file. 
***(map.h)***

The project accepts input from a user as part of the necessary operation of the program. 
***(interaction.h)***

### Object Oriented Programming

The project code is organized into classes with class attributes to hold the data, and class methods to perform tasks.
***(it's common to the project)***

All class data members are explicitly specified as public, protected, or private. 
***(it's common to the project)***

All class members that are set to argument values are initialized through member initialization lists.
***(player.h)***

All class member functions document their effects, either through function names, comments, or formal documentation. Member functions do not change program state in undocumented ways. 
***(it's common to the project)***

Appropriate data and functions are grouped into classes. Member data that is subject to an invariant is hidden from the user. State is accessed via member functions.
***(it's common to the project, extensivly at player.h)***

Inheritance hierarchies are logical. Composition is used instead of inheritance when appropriate. Abstract classes are composed of pure virtual functions. Override functions are specified. 
***(player.h)***

One function is overloaded with different signatures for the same function name. 
***(player.h - functions action(...))***

One member function in an inherited class overrides a virtual base class member function. 
***(player.h - function action() override)***

One function is declared with a template that allows it to accept a generic parameter. 
***(controller.h)***

### Memory Management

At least two variables are defined as references, or two functions use pass-by-reference in the project code. 
***(it's common to the project)***

At least one class that uses unmanaged dynamically allocated memory, along with any class that otherwise needs to modify state upon the termination of an object, uses a destructor. 
***(player.h)***

The project follows the Resource Acquisition Is Initialization pattern where appropriate, by allocating objects at compile-time, initializing objects when they are declared, and utilizing scope to ensure their automatic destruction. 
***(it's common to the project, constructors initialize required resources and destructors to release them)***
	
For all classes, if any one of the copy constructor, copy assignment operator, move constructor, move assignment operator, and destructor are defined, then all of these functions are defined. 
***(player.h)***

For classes with move constructors, the project returns objects of that class by value, and relies on the move constructor, instead of copying the object.
***(player.h)***

The project uses at least one smart pointer: unique_ptr, shared_ptr, or weak_ptr. The project does not use raw pointers.
***(game.h)***

### Concurrency

The project uses multiple threads in the execution.
***(game.cpp main thread, and worker threads: actionEnemy, actionPlayer)***

A promise and future is used to pass data from a worker thread to a parent thread in the project code.
***(game.cpp and interaction.h)***

A mutex or lock (e.g. std::lock_guard or `std::unique_lock) is used to protect data that is shared across multiple threads in the project code.
***(controller.h, game.cpp)***

A std::condition_variable is used in the project code to synchronize thread execution.
***(controller.h)***

# Compilation

### Windows:

Download and install MSYS2, add bin to System Path C:\msys64\mingw64\bin
Download and install CMake, add bin to System Path ...

```
cd game
mkdir build
cd build
cmake ..
mingw32-make
```

Manual compilation:

```
g++ .\src\game.cpp -IC:\msys64\mingw64\include -lmingw32 -mwindows -lSDL2main -lSDL2 -lopengl32 -pthread
```

### Linux:

```
git clone https://github.com/libsdl-org/SDL
cd SDL
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release --parallel

#CMake >= 3.15
sudo cmake --install . --config Release

#CMake <= 3.14
sudo make install
```

Compile Game manually:

```
cd game
mkdir build
cd build
cmake ..
make
```

# Map options
game is based on map at data/map.txt

you can modify X finish point location by setting it, means X elsewhere, also setting blank as 0 means cell is empty or 1 is obstacle

you can also create bigger map but keep x to y as 1 : 1, currently map has 20 x 20 cells, also window size is 1000 pixels, it scales itself

```
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,
0,0,0,0,0,E,0,0,0,0,0,1,1,1,1,1,1,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,
0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,
0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,1,0,0,X,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,
S,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
```

# How to run
double click a.exe
or build/game.exe

# Game rules

Player starts always at same locaton, after player action there is random Enemy action.

Player goal is to reach blue point (finish).

If Enemy will be rendered at same point, after Player action, Enemy wins.

# How to play

You should not meet enemy!
When you reach blue rectangle that's the goal!

Esc - end game

r - restart, when you get to blue point or enemy reaches you first

arrow up - up

arrow down - down

arrow left - left

arrow right - right


![Screenshot](picture.PNG)
