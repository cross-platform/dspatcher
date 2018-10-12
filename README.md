[![Build Status](https://travis-ci.org/MarcusTomlinson/DSPatcher.svg?branch=master)](https://travis-ci.org/MarcusTomlinson/DSPatcher)
[![Build status](https://ci.appveyor.com/api/projects/status/nyitfgo2f56dbs6w/branch/master?svg=true)](https://ci.appveyor.com/project/MarcusTomlinson/dspatcher/branch/master)

# DSPatcher
Cross-Platform Graphical Tool for DSPatch

This project requires Qt: https://www.qt.io/download

```
git clone https://github.com/MarcusTomlinson/DSPatcher.git
cd DSPatcher
git submodule update --init --recursive --remote
mkdir build
cd build
cmake ..
make
```

DSPatcher is a cross-platform graphical tool for building DSPatch circuits.

The DSPatcher project is made up of 2 parts:
* QtPatcher - A cross-platform circuit drawing library written in Qt C++.
* DSPatcher - A graphical tool based on QtPatcher for building DSPatch circuits.

*See also:*

DSPatch (https://github.com/MarcusTomlinson/DSPatch): A powerful C++ flow-based programming framework.

DSPatchables (https://github.com/MarcusTomlinson/DSPatchables): A DSPatch component repository.
