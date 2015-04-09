#!/bin/bash
dir=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

install_name_tool -id $dir/link/libDSPatch.dylib $dir/link/libDSPatch.dylib

for file in $dir/dspatchables/*.dylib; do
    install_name_tool -id $file $file
    install_name_tool -change $(cut -d ' ' -f 1 <<< $(otool -L $file | grep libDSPatch.dylib)) $dir/link/libDSPatch.dylib $file
done