[![Build Status](https://travis-ci.org/MarcusTomlinson/DSPatcher.svg?branch=master)](https://travis-ci.org/MarcusTomlinson/DSPatcher)
[![Build status](https://ci.appveyor.com/api/projects/status/nyitfgo2f56dbs6w/branch/master?svg=true)](https://ci.appveyor.com/project/MarcusTomlinson/dspatcher/branch/master)

# DSPatcher

Cross-Platform Graphical Tool for DSPatch.

The DSPatcher project is made up of 2 parts:
1. QtPatcher - A cross-platform circuit drawing library written in Qt C++.
2. DSPatcher - A graphical tool based on QtPatcher for building DSPatch circuits.


## Build

This project requires Qt: https://www.qt.io/download

```
git clone https://github.com/cross-platform/dspatcher.git
cd dspatcher
git submodule update --init --recursive --remote
mkdir build
cd build
cmake ..
make
```

- *If `cmake ..` fails, try `cmake -D CMAKE_PREFIX_PATH=<path/to/Qt/x.y.z/compiler> ..`.*
- *`cmake ..` will auto-detect your IDE / compiler. To manually select one, use `cmake -G`.*
- *When building for an IDE, instead of `make`, simply open the cmake generated project file.*


### See also:

DSPatch (https://github.com/cross-platform/dspatch): A powerful C++ dataflow framework.

DSPatchables (https://github.com/cross-platform/dspatchables): A DSPatch component repository.
