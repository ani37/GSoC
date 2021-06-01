# libIGL & Polyscope & Ponca

Demonstrates basic functionality and project setup for using Polyscope with libIGL.



## To download and build

```
git clone --recurse-submodules https://github.com/nmwsharp/libigl-polyscope-project-template.git
cd libigl-polyscope-project-template
mkdir build
cd build
cmake ..
make -j4

./bin/example ../bunnyhead.obj

./bin/example ../spot.obj
```

## Usage


Check out `src/main.cpp` to see how easy it is! There are only 12 lines of Polyscope code in the whole demo to generate this visualization.
