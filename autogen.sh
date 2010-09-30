#!/bin/sh

echo "Checking for CMake..."
which cmake || {
    echo "CMake is required to configure this project."
    echo "It is available at http://www.cmake.org"
    echo "or a package \"cmake\" on most distributions."
    exit 1
}

if [ "k$1" = "k" ] ; then

    echo "Creating and entering build directory"
    mkdir -p build || {
	echo "Failed to create a build directory"
	exit 1
    }
    cd build
    cmake .. && {
	echo " "
	echo "Successful configuration! Compile with:"
	echo "  cd build"
	echo "  make"
    }

fi


if [ "k$1" = "kmingw" ] ; then

    echo "Creating and entering build directory"
    mkdir -p mingw || {
	echo "Failed to create a build directory"
	exit 1
    }
    cd mingw
    echo "We assume any necessary mingw libraries are in $HOME/mingw/install"
    cmake  -DCMAKE_TOOLCHAIN_FILE=$PWD/../conf/mingwin.cmake -DCMAKE_EXE_LINKER_FLAGS="-L $HOME/mingw/install/lib" -DCMAKE_RC_OUTPUT_EXTENSION=".obj" .. && {
	echo " "
	echo "Successful configuration! Compile with:"
	echo "  cd mingw"
	echo "  make"
    }

fi



