[![Build Status](https://bit.ly/2GYsfv9)](https://travis-ci.org/cross-platform/dspatcher)
[![Build status](https://bit.ly/2OPL4qR)](https://ci.appveyor.com/project/MarcusTomlinson/dspatcher/branch/master)

[![Snap](https://bit.ly/2ZWfetD)](https://snapcraft.io/dspatcher)

# DSPatcher

Cross-Platform Graphical Tool for [DSPatch](https://github.com/cross-platform/dspatch).

The DSPatcher project is made up of 2 parts:
1. QtPatcher - A cross-platform circuit drawing library written in Qt C++.
2. DSPatcher - A graphical tool based on QtPatcher for building DSPatch circuits.

![Screenshot](https://bit.ly/33vlCuc)

## Build

This project requires Qt5: https://www.qt.io/download

```
git clone https://github.com/cross-platform/dspatcher.git
cd dspatcher
git submodule update --init --recursive --remote
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
```

- *If `cmake ..` fails, try `cmake -D CMAKE_PREFIX_PATH=<path/to/Qt/x.y.z/compiler> ..`.*
- *`cmake ..` will auto-detect your IDE / compiler. To manually select one, use `cmake -G`.*
- *When building for an IDE, instead of `make`, simply open the cmake generated project file.*


### See also:

DSPatch (https://github.com/cross-platform/dspatch): A powerful C++ dataflow framework.

DSPatchables (https://github.com/cross-platform/dspatchables): A DSPatch component repository.
