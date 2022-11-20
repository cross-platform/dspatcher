#!/bin/bash

DIR=$(cd `dirname $0` && pwd)
cd $DIR/..

find . -iname *.h -o -iname *.cpp | grep -v build | grep -v builddir | grep -v DSPatchables | xargs clang-format --style=file --verbose -i
