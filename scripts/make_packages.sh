#!/bin/bash

while [ ! "k$1" = "k" ]; do 

    cfg="$HOME/.COOPY_PATHS"
    
    if [ ! -e $cfg ] ; then
	echo "Need: $cfg"
	echo "of form"
	echo "  MINGW_REPO=/home/paulfitz/cvs/coopy/"
	echo "  MINGW_BUILD=/home/paulfitz/cvs/coopy/mingw"
	echo "  LINUX_CHROOT=/scratch/debian-etch-i386"
	echo "  LINUX_CHROOT_REPO=/home/paulfitz/coopy"
	echo "  LINUX_CHROOT_BUILD=/home/paulfitz/coopy/build"
	echo "  OUTPUT=/tmp/coopy"
	exit 1
    fi
    
    source $cfg
    
    OUTPUT="$OUTPUT/coopy-pack"
    rm -rf $OUTPUT
    mkdir -p $OUTPUT
    
    if [ "k$1" = "kmingw" ]; then
	echo "Working on MINGW"
	cd $MINGW_REPO || ext 1
	git pull || exit 1
	cd $MINGW_BUILD || exit 1
	cmake . || exit 1
	make || exit 1
	rm -f *.exe
	make package || exit 1
	cp -v *.exe $OUTPUT
    fi

    if [ "k$1" = "klinux" ]; then
	cd $LINUX_CHROOT || exit 1
	cd ./$LINUX_CHROOT_REPO || exit 1
	echo "Working on linux repo: $PWD"
	sudo git pull
	cd $LINUX_CHROOT
	cd ./$LINUX_CHROOT_BUILD || exit 1
	sudo rm -rf *.tgz *.sh *.Z
	(
	    echo "cd $LINUX_CHROOT_BUILD"
	    echo "cmake ."
	    echo "make"
	    echo "make package"
	    echo "make source_package"
	    ) | sudo chroot $LINUX_CHROOT
	cp -v *.tgz *.sh *.Z $OUTPUT
    fi

    shift

done
