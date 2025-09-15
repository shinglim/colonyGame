cmake_minimum_required(VERSION 3.10)
project(MiniRimWorld)

set(CMAKE_CXX_STANDARD 17)

# Source files
file(GLOB SRC
        src/*.cpp
)

add_executable(MiniRimWorld ${SRC}
        src/Window.cpp
        src/Window.h
        src/MainMenu.cpp
        src/MainMenu.h
        src/structures.h
        src/structures.cpp
        src/Game.h
        src/Game.cpp
        src/enums.h
        src/Character.cpp
        src/Character.h
        src/Cities.cpp
        src/Cities.h
        src/Army.cpp
        src/Army.h
        src/Caravan.cpp
        src/Caravan.h
        src/enums.cpp)

# --- Find SDL2 ---
if(WIN32)
    # Windows: set this to your SDL2 path
    set(SDL2_PATH "C:/Libraries/SDL2")  # Change to your SDL2 location
    include_directories(${SDL2_PATH}/include)
    link_directories(${SDL2_PATH}/lib)
    target_link_libraries(MiniRimWorld SDL2 SDL2main)

elseif(APPLE)
    # macOS: Use Homebrew SDL2
    # brew install sdl2
    find_package(SDL2 REQUIRED)
    include_directories(${SDL2_INCLUDE_DIRS})
    target_link_libraries(MiniRimWorld ${SDL2_LIBRARIES})

else()
    # Linux
    find_package(SDL2 REQUIRED)
    include_directories(${SDL2_INCLUDE_DIRS})
    target_link_libraries(MiniRimWorld ${SDL2_LIBRARIES})
endif()
