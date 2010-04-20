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
	echo "  LINUX_CHROOT_BUILD=/home/paulfitz/coopy_build"
	echo "  OUTPUT=/tmp/coopy"
	exit 1
    fi
    
    source $cfg
    
    OUTPUT="$OUTPUT/coopy-pack"
    mkdir -p $OUTPUT
    
    if [ "k$1" = "kmingw" ]; then
	echo "Working on MINGW"
	cd $MINGW_REPO || ext 1
	git pull || exit 1
	cd $MINGW_BUILD || exit 1
	cmake . || exit 1
	make || {
	    make clean
	    make || exit 1
	}
	rm -f *.exe
	make package || exit 1
	cp -v *.exe $OUTPUT

	# Give a no-install version
	cp -v bin/coopy.exe $OUTPUT/`echo coopy-*.exe | sed "s/.exe/-direct.exe/"`
    fi

    if [ "k$1" = "klinux" ]; then
	cd $LINUX_CHROOT || exit 1
	cd ./$LINUX_CHROOT_REPO || exit 1
	echo "Working on linux repo: $PWD"
	sudo git pull
	cd $LINUX_CHROOT
	cd ./$LINUX_CHROOT_BUILD || exit 1
	test -e static_libs || (
	    echo "Static libraries not set up"
	    exit 1
	    )
	sudo rm -rf *.gz *.sh *.Z
	(
	    echo "cd $LINUX_CHROOT_BUILD"
	    echo "cmake ."
	    echo "make || ( make clean; make )"
	    echo "make package"
	    echo "make package_source"
	    ) | sudo chroot $LINUX_CHROOT
	cp -v *.gz *.sh *.Z $OUTPUT
    fi

    shift

done
