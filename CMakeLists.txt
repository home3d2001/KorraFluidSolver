##################
#     Korra     #
##################

# credit - base CMake config : Yining Karl Li , edited CMake config: Akshay Shah & Debanshu Singh

#name your project
project(Korra)
cmake_minimum_required(VERSION 2.8)

# include source directories
include_directories(
        src/
)

# set creates a variable
set (NUPARU "nuparu")
# add include and src directories to path
include_directories(
        ${NUPARU}/include
        ${NUPARU}/src
)

# Add path for pre-compiled libraries here (we will later link them with our compiled source)
# Add Nuparu library to path for OSX, linux and windows
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${NUPARU}/lib/osx)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${NUPARU}/lib/linux /usr/lib64)
elseif(WIN32)
    set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${NUPARU}/lib/win)
endif()

# set creates a variable
set (THIRDPARTY "src/thirdparty")
# add include and src directories to path
include_directories(
        ${THIRDPARTY}/nanogui/include
        ${THIRDPARTY}/nanogui/src
)

# NanoGUI library path
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${THIRDPARTY}/nanogui)

#set include variables
set(GLFW_INCLUDE_DIR ${NUPARU}/include )
set(GLEW_INCLUDE_DIR ${NUPARU}/include )
set(GLFW_LIBRARY_DIR ${CMAKE_LIBRARY_PATH})
set(GLEW_LIBRARY_DIR ${CMAKE_LIBRARY_PATH})

# Use find_package & find_library to link with
find_package(OPENGL REQUIRED)
find_package(GLEW)
find_library(GLFW_LIBRARY "glfw3" HINTS ${GLFW_LIBRARY_DIR})
find_library(JSONCPP "jsoncpp")
find_library(TBB "tbb")
# link half for openvdb
find_library(HALF "half")
find_library(OPENVDB "openvdb")
# find_library(PARTIO "partio")
find_library(NANOGUI "nanogui")

add_definitions(
        -DTW_STATIC
        -DTW_NO_LIB_PRAGMA
        -DTW_NO_DIRECT3D
        -DGLEW_STATIC
        -D_CRT_SECURE_NO_WARNINGS
)

set(CORE_LIBS ${GLFW_LIBRARY} ${GLUT_LIBRARY} ${GLEW_LIBRARY} ${JSONCPP} ${TBB} ${HALF} ${OPENVDB} ${NANOGUI} ${PARTIO} ${ZILB} ${OPENGL_LIBRARY} )

# OSX-specific hacks/fixes
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    find_library(COCOA Cocoa)
    find_library(COREVIDEO CoreVideo)
    find_library(IOKIT IOKit)
    find_library(COREFOUNDATION CoreFoundation)
    find_library(CARBON_FRAMEWORK Carbon)

    set(CORE_LIBS ${CORE_LIBS} "-framework Cocoa -framework IOKit -framework CoreFoundation -framework CoreVideo -framework Carbon")

endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

# Linux specific hacks/fixes
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lX11 -lXxf86vm -lXrandr -lpthread -lXi")
endif()

# set compiler flags for c++11
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux" OR ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -m64 -msse2 -w")
elseif(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

if(MSVC)
    set(COMPILER_FLAGS
            CMAKE_CXX_FLAGS
            CMAKE_CXX_FLAGS_DEBUG
            CMAKE_CXX_FLAGS_RELEASE
            CMAKE_C_FLAGS
            CMAKE_C_FLAGS_DEBUG
            CMAKE_C_FLAGS_RELEASE
        )
endif()

# Add source files you want to compile (.cpp)
set(CORE_SRC
        src/loadShaders.cpp
        src/utility.cpp
        src/main.cpp
        src/shaderProgram/shaderProgram.cpp
        src/shaderProgram/particleAdvectProgram.cpp
        src/input/keyboardControl.cpp
        src/viewer.cpp
        src/camera/camera.cpp
        src/geometry/geometry.cpp
        src/geometry/box.cpp
        src/geometry/fluidGeo.cpp
        src/fluidSolver/fluidParticle.cpp
        src/fluidSolver/fluidSolver.cpp
        src/fluidSolver/SPHGrid.cpp
        src/fluidSolver/SPHSolver.cpp
        src/scene/scene.cpp
)

add_executable(Korra ${CORE_SRC})
target_link_libraries(Korra ${CORE_LIBS})

# Include zlib for osx
find_package( ZLIB REQUIRED )
if ( ZLIB_FOUND )
    include_directories( ${ZLIB_INCLUDE_DIRS} )
    target_link_libraries( Korra ${ZLIB_LIBRARIES} )
endif( ZLIB_FOUND )

