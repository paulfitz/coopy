#!/bin/bash

BUILD_DIR=$PWD

source ./settings.sh || {
	echo "No settings.sh found, are we in the build directory?"
	exit 1
}

platform=$1
dir=$2

if [ "k$dir" = "k" ]; then
    echo "Call as: build_coopy.sh <platform> <dir>"
    exit 1
fi

source chroot_${platform}.sh || {
	echo "Cannot find corresponding chroot"
	exit 1
}

mkdir -p $dir
cd $dir

if [ ! -e "build_chroot" ]; then
    sudo cp -R $CHROOT_DIR build_chroot || exit 1
fi

function run_in_chroot {
    echo "Running [$2]"
    sudo chroot $1 bash -c "$2"
}

run_in_chroot build_chroot "yes | apt-get install wget zlib1g-dev git-core cmake" || exit 1
# run_in_chroot build_chroot "yes | apt-get install libx11-dev zlib1g-dev libwxgtk2.8-dev git-core cmake" || exit 1


run_in_chroot build_chroot "cd /tmp; test -e coopy || git clone git://github.com/paulfitz/coopy.git" || exit 1
CHROOT_SRC=/tmp/coopy
CHROOT_BUILD=/tmp/coopy/build
run_in_chroot build_chroot "cd $CHROOT_SRC && git pull" || exit 1
run_in_chroot build_chroot "mkdir -p $CHROOT_BUILD" || exit 1

CMAKE=cmake

# Fetch cmake if version in repository is too old
if [ -e build_chroot/$CHROOT_BUILD/local_cmake ]; then
    CMAKE=`cd build_chroot/$CHROOT_BUILD/; echo cmake-*/bin/cmake`
else
    run_in_chroot build_chroot "cd $CHROOT_BUILD && cmake --version | grep ' 2\.4' && ( wget http://www.cmake.org/files/v2.6/cmake-2.6.4-Linux-i386.tar.gz && tar xzvf cmake-2.6.4-Linux-i386.tar.gz && touch local_cmake )"
    run_in_chroot build_chroot "yes | apt-get install ia32-libs"
fi

# If we downloaded cmake, we need to make sure we have 32-bit libraries
if [ -e build_chroot/$CHROOT_BUILD/local_cmake ]; then
    CMAKE=$CHROOT_BUILD/`cd build_chroot/$CHROOT_BUILD/; echo cmake-*/bin/cmake`
fi

run_in_chroot build_chroot "mkdir -p $CHROOT_BUILD && cd $CHROOT_BUILD && rm -f CMakeCache.txt && $CMAKE -DCREATE_SHARED_LIBRARIES:BOOL=FALSE $CHROOT_SRC" || exit 1
run_in_chroot build_chroot "cd $CHROOT_BUILD && make" || exit 1
run_in_chroot build_chroot "cd $CHROOT_BUILD && $CMAKE -DCPACK_GENERATOR='DEB' -DCPACK_PACKAGE_CONTACT='paul@robotrebuilt.com' -DCPACK_DEBIAN_PACKAGE_MAINTAINER='paul@robotrebuilt.com' ." || exit 1
# run_in_chroot build_chroot "cd $CHROOT_BUILD && $CMAKE -DCPACK_GENERATOR='DEB' -DCPACK_PACKAGE_CONTACT='paul@robotrebuilt.com' -DCPACK_DEBIAN_PACKAGE_MAINTAINER='paul@robotrebuilt.com' -DCPACK_DEBIAN_PACKAGE_DEPENDS:STRING='libwxgtk2.8-0 (>= 2.8.7)' ." || exit 1
run_in_chroot build_chroot "cd $CHROOT_BUILD && rm -f *.deb && make package" || exit 1

rm -f *.deb
cp build_chroot/$CHROOT_BUILD/coopy-*.deb . || exit 1
fname=`ls *.deb`

(
	echo "export COOPY_PACKAGE_DIR='$PWD'"
	echo "export COOPY_PACKAGE='$fname'"
) > $BUILD_DIR/coopy_${platform}.sh

echo "To enter chroot, run"
echo "  sudo chroot $PWD/build_chroot"
