# Korra Fluid Solver, CIS 563, University of Pennsylvania, Spring 2016

## Update March 30 (Sprint 2)

### Completed features:
Pressure force field
Viscosity force field (change value inside scene.json)
Press Space to pause simulation
z-index sorting

Currently my simulation is running on the CPU still so it's pretty slow. The only tbb call is in SPHSolver::Update (in SPHSolver.cpp), that update the particle velocity & position using parallel_for (not even sure if this is worth any extra credits);

### Incomplete features:
- OpenVDB
- TBB

## Update March 23 (Sprint 1)
I implemented the SPH Solver with naive neighbor search. When you started running the code, you'll see a test particle iterating through the grid and shows all the neighbors in red color. However, this neighbor search isn't using a working uniform grid yet.

I have a simple test suite in main.cpp to test Poly6. Currently spiky kernel and viscous kernel are not tested. By uncommenting `#define UNIT_TEST`, there should be print out of the kernel unit testing.

## Overview

__Korra__ is a cross-platform C++ fluid SPH solver, designed and built for CIS563 course at the University of Pennsylvania, Sprin 2016, instructed by [Debanshu Singh](http://www.debanshusingh.com/). __Korra__ uses OpenGL heavily for the graphics pipeline. It is also designed for GPU optimization for fluid advection.

Details will be updated as more features are coming soon.

## Automatic build (easy!)
```
./install.sh
```
The application `Korra` will be created in build folder. Simply run `./Korra` to launch.

## Manual build (if you prefer doing it manually)

1. Clone using git clone --recursive git@github.com:trungtle/KorraFluidSolver.git

If you forgot to call --recursive, you can recover the submodules by
```
cd CIS563-FluidSolver (at top directory)
git submodule update --init --recursive
```

There should be 2 submodules for `jsoncpp` and `nanogui`

2. Compile nanogui with cmake and make
```
cd src/thirdparty/nanogui
cmake -G "Unix Makefiles" .
make
```

3. Compile Korra with cmake and make
```
cd ../../../ (at top directory)
mkdir build
cd build
cmake -G "Unix Makefiles" ../
make

4. Launch
```
./Korra
```



## Classes

The following are classes for the fluid solver.

### Viewer

The _Viewer_ class contains all the OpenGL, glfw, glew, shader programs, and scene initialization. Once the the initialization is completed, the _Viewer_ kicks the simulation loop. The loop can be canceled by hitting the ESC key.

- __Note__: By defining **#define TEST_SCENE** in `viewer.h` and `scene.h`, the test scene would be used instead.

### Scene

The _Scene_ class contains the _Camera_ and all other objects in the scene. The _Scene_ class takes in a json file in the `scene/` directory for initializing the fluid solver. In this case, it's the `scene.json` file.

There are three objects of our interests:

- *m_fluidContainer*, a _Box_ object that specifies the boundary of the fluid.
- *m_fluidGeo*, a _FluidGeo_ object that specifies the particles of the fluid.
- *m_fluidSolver*, a _FluidSolver_ object that gets updated every simulation frame to compute the fluid state.

- __Note__: By defining **#define TEST_SCENE** in `viewer.h` and `scene.h`, the test scene would be used instead.

### ShaderProgram

The _ShaderProgram_ compiles the `glsl/simple.vert` vertex shader and the `glsl/simple.frag`. This program is used to draw simple geometry without any special shading.

### ParticleAdvectProgram

The _ParticleAdvectProgram_ compiles the `glsl/particle_advect.vert`, `glsl/particle_advect.frag`, `glsl/particle_draw.frag`, and `glsl/particle_draw.frag` into 2 programs: `m_programAdvect` and `m_programDraw`.

This class is used to perform two render passes for transform feedback. In short, the first pass uses the `particle_advect.vert` to update particle positions but skipping the rasterization phase, passing its vertex attributes directly to the `partice_draw.vert` and `particle_draw.frag` for the actual drawing. This approaches allow us to parallelize the particle update using only the shaders.

### Camera

The _Camera_ class handles the view matrix. The _Camera_ class uses the _KeyboardControl_ class to handles user keyboard input. To control the _Camera_ movement, uses:

- **W**: Pan up
- **S**: Pan down
- **A**: Pan left
- **D**: Pan right
- **Up**: Look up
- **Down**: Look down
- **Left**: Look left
- **Right**: Look right

### Geometry

The _Geometry_ is the base class for all the drawable geometry classes in a scene. It is not meant to be instanced directly but to be derived by new classes (etc. _Box_ and _FluidGeo_). _Geometry_ class contains all the information for transformations, vertex buffers, vertex arrays, index buffer, and drawing mode.

Each new geometry instanced **NEEDS** to call Create() in order to properly initialize its data.

This class supports the basic affine transformation:

- Translate
- Rotate
- Scale

This class supports three drawing mode:

- Wireframe (GL_LINES)
- Shaded (GL_TRIANGLES)
- Vertex (GL_POINTS)

### Box

_Box_ is derived from _Geometry_. _Box_ represents a simple box geometry.

### FluiGeo

_FluiGeo_ is derived from _Geometry_. FluiGeo represents a collection of fluid particles. _FluidGeo_ also contains the particle positions and velocities.

### FluidSolver

_FluidSolver_ initializes all the particle information such as container boundary and particle separation. Its `Update()` function is called each frame to update the solver if needed.

### KeyboardControl

_KeyboardControl_ handles user keyboard input.

## Code flow

Starting with `main.cpp`, the _Viewer_ is initialized, then the simulation loop kicks in. For each frame, the _Scene_ calls its `Update()` and `Draw()` functions to update the state of the solver then render the result.

## Extra features

N/A

## Camera control

See Camera class section above.

## LICENSE

See LICENSE.md for the MIT License.

## Third Party

- CIS277, Camera code
- glfw, [Website](http://www.glfw.org/)
- glew, [Website](http://glew.sourceforge.net/)
- glm, [Website](http://glm.g-truc.net/0.9.7/index.html)
- opengl-tutorial.org, [Website](http://www.opengl-tutorial.org/)
- jsoncpp, [Github](https://github.com/open-source-parsers/jsoncpp)
- tbb, [Download](https://www.threadingbuildingblocks.org/)
- easylogging++, [Github](https://github.com/easylogging/easyloggingpp
)
- nanogui, [Github](https://github.com/wjakob/nanogui)
